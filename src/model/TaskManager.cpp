#include "TaskManager.h"
//#include "logging/Log.h"
#include "utilities/Convert.h"

namespace model {
TaskManager::TaskManager(const std::shared_ptr<IdGenerator>& generator) :
        generator_{generator} {}

bool TaskManager::AddTask(const Task& task) {
    std::unique_lock lock(mutex_);
    TaskId id{generator_->GenerateId()};

    if (tasks_.find(id) != tasks_.end()) {
//        LOG(error, "IdGenerator returns the id {" + std::to_string(id.value()) + "}, that is already in use.");
        return false;
    }
    tasks_.insert({id, CreateHierarchicalTask(task, std::nullopt)});

//    LOG(debug, "Task with {" + task.ShortDebugString() + "} added.");
    return true;
}

bool TaskManager::AddSubTask(const Task& task, const TaskId& parent) {
    std::unique_lock lock(mutex_);
    if (tasks_.find(parent) == tasks_.end() || tasks_[parent].has_parent() ||
        tasks_[parent].task().status() == Task_Status_COMPLETED)
        return false;

    TaskId id{generator_->GenerateId()};

    tasks_.insert({id, CreateHierarchicalTask(task, parent)});
    return true;
}

bool TaskManager::Edit(const TaskId& id, const Task& task) {
    std::unique_lock lock(mutex_);
    if (tasks_.find(id) == tasks_.end() || tasks_[id].task().status() == Task_Status_COMPLETED)
        return false;

    tasks_[id].set_allocated_task(std::make_unique<Task>(task).release());
    return true;
}

bool TaskManager::Complete(const TaskId& id) {
    std::unique_lock lock(mutex_);
    if (tasks_.find(id) == tasks_.end())
        return false;

    tasks_[id].mutable_task()->set_status(Task_Status_COMPLETED);
    for (const auto &[child_id, task]: tasks_)
        if (task.has_parent() && task.parent() == id)
            tasks_[child_id].mutable_task()->set_status(Task_Status_COMPLETED);

    return true;
}

bool TaskManager::Delete(const TaskId& id) {
    std::unique_lock lock(mutex_);
    if (tasks_.find(id) == tasks_.end())
        return false;

    for (auto it = tasks_.begin(); it != tasks_.end();)
        ((*it).second.has_parent() && (*it).second.parent() == id) ? tasks_.erase(it++) : ++it;

    tasks_.erase(id);

    return true;
}

ManyTasksWithId TaskManager::ShowByLabel(const std::string& label, const TasksSortBy& sort_by) {
    ManyTasksWithId result;
    {
        std::shared_lock lock(mutex_);
        for (const auto &[id, task]: tasks_) {
            auto begin = task.task().labels().begin();
            auto end = task.task().labels().end();
            if (std::find(begin, end, label) != end && task.task().status() != Task_Status_COMPLETED) {
                result.mutable_tasks()->Add(CreateTaskWithId(id, task.task()));
            }
        }
    }

    {
        std::unique_lock lock(mutex_);
//        LOG(debug, "Array from " + std::to_string(result.tasks_size()) + " tasks returned");
    }

    SortTasksWithId(result, sort_by);
    return result;
}

ManyTasksWithId TaskManager::ShowParents(const TasksSortBy& sort_by) {
    ManyTasksWithId result;
    {
        std::shared_lock lock(mutex_);
        for (const auto &[id, task]: tasks_)
            if (!task.has_parent() && task.task().status() != Task_Status_COMPLETED)
                result.mutable_tasks()->Add(CreateTaskWithId(id, task.task()));
    }

    SortTasksWithId(result, sort_by);
    return result;
}

CompositeTask TaskManager::ShowTask(const TaskId& parent, const TasksSortBy& sort_by) {
    CompositeTask result;
    ManyTasksWithId child;
    {
        std::shared_lock lock(mutex_);
        if (!tasks_.count(parent))
            return result;

        for (const auto &[id, task]: tasks_)
            if (task.has_parent() && task.parent() == parent && task.task().status() != Task_Status_COMPLETED)
                child.mutable_tasks()->Add(CreateTaskWithId(id, task.task()));

        result.set_allocated_task(
                std::make_unique<TaskWithId>(CreateTaskWithId(parent, tasks_.at(parent).task())).release());
    }
    SortTasksWithId(child, sort_by);

    result.mutable_children()->Add(child.tasks().begin(), child.tasks().end());
    return result;
}

ManyCompositeTasks TaskManager::ShowAll(const TasksSortBy& sort) {
    ManyCompositeTasks result;
    ManyTasksWithId parents = ShowParents(sort);

    for (const auto& parent: parents.tasks())
        result.mutable_tasks()->Add(ShowTask(parent.id(), sort));
    return result;
}

ManyHierarchicalTasks TaskManager::GetAllTasks() {
    ManyHierarchicalTasks tasks;
    {
        std::shared_lock lock(mutex_);
        std::copy(tasks_.begin(), tasks_.end(), std::back_inserter(tasks));
    }
    return tasks;
}

void TaskManager::Overwrite(const ManyHierarchicalTasks& tasks) {
    std::unique_lock lock(mutex_);
    tasks_.clear();
    for (const auto& task: tasks)
        tasks_.insert(task);

    int max_id = 0;
    if (!tasks_.empty())
        max_id = (*prev(tasks_.end())).first.value();

    generator_ = std::make_shared<IdGenerator>(max_id);
}
}