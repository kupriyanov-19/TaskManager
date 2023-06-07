#include "CreateProtoObjects.h"

TaskId CreateTaskId(int value) {
    TaskId id;
    id.set_value(value);
    return id;
}

Task CreateTask(const std::string& title, Task_Priority priority,
                time_t date, const std::vector<std::string>& labels, Task_Status status) {
    Task task;
    task.set_title(title);
    task.set_priority(priority);
    google::protobuf::Timestamp time;
    time.set_seconds(date);
    task.set_allocated_date(std::make_unique<google::protobuf::Timestamp>(time).release());
    for (const auto& label: labels)
        task.add_labels(label);
    task.set_status(status);
    return task;
}

TaskWithId CreateTaskWithId(const TaskId& id, const Task& task) {
    TaskWithId task_with_id;
    task_with_id.set_allocated_task(std::make_unique<Task>(task).release());
    task_with_id.set_allocated_id(std::make_unique<TaskId>(id).release());
    return task_with_id;
}

HierarchicalTask CreateHierarchicalTask(const Task& task, std::optional<TaskId> parent) {
    HierarchicalTask hierarchical_task;
    hierarchical_task.set_allocated_task(std::make_unique<Task>(task).release());
    if (parent.has_value())
        hierarchical_task.set_allocated_parent(std::make_unique<TaskId>(parent.value()).release());
    return hierarchical_task;
}