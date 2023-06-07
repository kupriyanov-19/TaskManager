#pragma once

#include <map>
#include <memory>
#include <vector>
#include <optional>
#include <shared_mutex>
#include "Task.pb.h"
#include "TaskStructures.pb.h"
#include "IdGenerator.h"
#include "utilities/ComparisonProtoObjects.h"
#include "utilities/CreateProtoObjects.h"
#include "utilities/SortingProtoObjects.h"

namespace model {
using ManyHierarchicalTasks = std::vector<std::pair<TaskId, HierarchicalTask>>;

class TaskManager {
public:
    explicit TaskManager(const std::shared_ptr<IdGenerator>& generator);
    virtual ~TaskManager() = default;

    virtual bool AddTask(const Task& task);
    virtual bool AddSubTask(const Task& task, const TaskId& id);
    virtual bool Edit(const TaskId& id, const Task& task);
    virtual bool Complete(const TaskId& id);
    virtual bool Delete(const TaskId& id);

    virtual ManyTasksWithId ShowByLabel(const std::string& label, const TasksSortBy&);
    virtual ManyTasksWithId ShowParents(const TasksSortBy&);
    virtual CompositeTask ShowTask(const TaskId& id, const TasksSortBy&);
    virtual ManyCompositeTasks ShowAll(const TasksSortBy&);

    virtual ManyHierarchicalTasks GetAllTasks();
    virtual void Overwrite(const ManyHierarchicalTasks&);
private:

    std::map<TaskId, HierarchicalTask> tasks_;
    std::shared_ptr<IdGenerator> generator_;
    std::shared_mutex mutex_;
};
}