#pragma once

#include <vector>
#include <string>
#include <optional>
#include <fstream>
#include <shared_mutex>
#include "Task.pb.h"
#include "TaskStructures.pb.h"

using ManyHierarchicalTasks = std::vector<std::pair<TaskId, HierarchicalTask>>;

class TaskPersister {
public:
    explicit TaskPersister(const std::string& filename, const std::string &password) : filename_{filename}, password_{password} {}

    virtual bool Save(const ManyHierarchicalTasks& tasks);
    virtual std::optional<ManyHierarchicalTasks> Load();

    virtual ~TaskPersister() = default;
private:
    const std::string filename_;
    const std::string password_;
    std::shared_mutex mutex_;
};

