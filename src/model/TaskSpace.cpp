#include "TaskSpace.h"

namespace model {

bool TaskSpace::AddTask(const Task& task) {
    return manager_->AddTask(task);
}

bool TaskSpace::AddSubTask(const Task& task, const TaskId& id) {
    return manager_->AddSubTask(task, id);
}

bool TaskSpace::Edit(const TaskId& id, const Task& task) {
    return manager_->Edit(id, task);
}

bool TaskSpace::Complete(const TaskId& id) {
    return manager_->Complete(id);
}

bool TaskSpace::Delete(const TaskId& id) {
    return manager_->Delete(id);
}

ManyTasksWithId TaskSpace::ShowByLabel(const std::string& label, const TasksSortBy& sort_by) {
    return manager_->ShowByLabel(label, sort_by);
}

ManyTasksWithId TaskSpace::ShowParents(const TasksSortBy& sort_by) {
    return manager_->ShowParents(sort_by);
}

CompositeTask TaskSpace::ShowTask(const TaskId& id, const TasksSortBy& sort_by) {
    return manager_->ShowTask(id, sort_by);
}

ManyCompositeTasks TaskSpace::ShowAll(const TasksSortBy& sort_by) {
    return manager_->ShowAll(sort_by);
}

bool TaskSpace::Save(const std::string& name) {
    std::vector<std::pair<TaskId, HierarchicalTask>> tasks{manager_->GetAllTasks()};
    TaskPersister persister{name_, password_};
    if (!persister.Save(tasks)) return false;
    else return true;
}

bool TaskSpace::Load(const std::string& name) {
    TaskPersister persister{name_, password_};
    std::optional<std::vector<std::pair<TaskId, HierarchicalTask>>> tasks{persister.Load()};
    if (!tasks.has_value())
        return false;
    manager_->Overwrite(tasks.value());
    return true;
}

bool TaskSpace::CreateSpace(const std::string& name, const std::string& password) {
    std::ifstream file(name);
    if (!file.fail()) return false;
    file.close();
    name_=name; password_=password;
    manager_->Overwrite({});
    TaskPersister persister{name_, password_};
    return persister.Save({});
}

bool TaskSpace::EnterSpace(const std::string& name, const std::string& password) {
    if (Load("")) {
        name_=name; password_=password;
        return true;
    }
    else return false;
}
}