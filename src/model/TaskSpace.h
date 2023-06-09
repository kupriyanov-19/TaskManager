#pragma once

#include "TaskManager.h"
#include "TaskSpace.h"
#include "persistence/TaskPersister.h"

namespace model {

/**
 * \brief Manager of core
 * \details Manages \b TaskManager and \b TaskPersister
 * \author Michael Kupriianov
 */
class TaskSpace {
public:
    /**
     * Constructor that creates an instance of the model
     * @param manager - pointer to \b TaskManager instance
     */
    explicit TaskSpace(const std::shared_ptr<model::TaskManager>& manager) :
            manager_{manager} {}
    /**
     * Virtual destructor for correct inheritance
     */
    virtual ~TaskSpace() = default;

    /**
     * Adds a new task
     * @param task - protobuf message containing the task to be added
     * @return \b bool - is addition successful
     */
    virtual bool AddTask(const Task& task);
    /**
     * Adds a new subtask with exiting parent
     * @param task - protobuf message containing the subtask to be added
     * @param id - protobuf message containing the id of parent
     * @return \b bool - is addition successful
     */
    virtual bool AddSubTask(const Task& task, const TaskId& id);
    /**
     * Edits existing task
     * @param id - protobuf message containing id of task to be edited
     * @param task - protobuf message containing new content of task
     * @return \b bool - is edition successful
     */
    virtual bool Edit(const TaskId& id, const Task& task);
    /**
     * Completes existing task
     * @param id - protobuf message containing id of task to be completed
     * @return \b bool - is completion successful
     * @warning All subtasks of this task will also be completed
     */
    virtual bool Complete(const TaskId& id);
    /**
     * Deletes existing task
     * @param id - protobuf message containing id of task to be deleted
     * @return \b bool - is deletion successful
     * @warning All subtasks of this task will also be deleted
     */
    virtual bool Delete(const TaskId& id);

    /**
     * Shows all tasks containing the given label
     * @param label - string with label
     * @param sort_by - enum which informs by what parameter sort requested tasks
     * @return \b ManyTasksWithId - protobuf message containing requesting tasks
     */
    virtual ManyTasksWithId ShowByLabel(const std::string& label, const TasksSortBy& sort_by);
    /**
     * Shows all tasks expect subtasks
     * @param sort_by - enum which informs by what parameter sort requested tasks
     * @return \b ManyTasksWithId - protobuf message containing requesting tasks
     */
    virtual ManyTasksWithId ShowParents(const TasksSortBy& sort_by);
    /**
     * Shows task with given id with ith subtasks
     * @param id - protobuf message containing id of task to be shown
     * @param sort_by - enum which informs by what parameter sort children of requesting task
     * @return \b CompositeTask - protobuf message containing requesting task
     */
    virtual CompositeTask ShowTask(const TaskId& id, const TasksSortBy& sort_by);
    /**
     * Shows all tasks with subtasks
     * @param sort_by - enum which informs by what parameter sort requested tasks
     * @return \b ManyCompositeTasks - protobuf message containing requesting tasks
     */
    virtual ManyCompositeTasks ShowAll(const TasksSortBy& sort_by);

    virtual bool Save(const std::string& name);

    virtual bool Load(const std::string& name);

    virtual bool CreateSpace (const std::string& name, const std::string& password);

    virtual bool EnterSpace (const std::string& name, const std::string& password);
private:
    const std::shared_ptr<TaskManager> manager_;
    std::string name_;
    std::string password_;
};
}

