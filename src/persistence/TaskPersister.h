#pragma once

#include <vector>
#include <string>
#include <optional>
#include <shared_mutex>
#include "Task.pb.h"
#include "TaskStructures.pb.h"

using ManyHierarchicalTasks = std::vector<std::pair<TaskId, HierarchicalTask>>;

class TaskPersister {
public:
    explicit TaskPersister(const std::string& filename) : filename_{filename} {}

    virtual bool Save(const ManyHierarchicalTasks& tasks);
    virtual std::optional<ManyHierarchicalTasks> Load();

    virtual ~TaskPersister() = default;
private:
    const std::string filename_;
    std::shared_mutex mutex_;
};

