#pragma once

#include "Task.pb.h"
#include "TaskStructures.pb.h"

namespace ui {
/**
 * \brief Model controller interface
 * \details Passes requests to \b Model
 * \author Michael Kupriianov
 */
class Controller {
public:
    /**
     * Virtual destructor for correct inheritance
     */
    virtual ~Controller() = default;

    /**
     * Sends request to adds a task
     * @param task - protobuf message containing the task to be added
     * @return \b bool - is addition successful
     */
    virtual bool AddTask(const Task& task) = 0;
    /**
     * Sends request to adds a subtask with given id of parent
     * @param task - protobuf message containing the subtask to be added
     * @param id - protobuf message containing the id of parent
     * @return \b bool - is addition successful
     */
    virtual bool AddSubTask(const Task& task, const TaskId& id) = 0;
    /**
     * Sends request to edits a task
     * @param id - protobuf message containing id of task to be edited
     * @param task - protobuf message containing new content of task
     * @return \b bool - is edition successful
     */
    virtual bool Edit(const TaskId& id, const Task& task) = 0;
    /**
     * Sends request to completes task with given id
     * @param id - protobuf message containing id of task to be completed
     * @return \b bool - is completion successful
     */
    virtual bool Complete(const TaskId& id) = 0;
    /**
     * Sends request to deletes task with given id
     * @param id - protobuf message containing id of task to be deleted
     * @return \b bool - is deletion successful
     */
    virtual bool Delete(const TaskId& id) = 0;

    /**
     * Sends request to shows all tasks containing the given label
     * @param label - string with label
     * @param sort_by - enum which informs by what parameter sort requested tasks
     * @return \b ManyTasksWithId - protobuf message containing requesting tasks
     */
    virtual ManyTasksWithId ShowByLabel(const std::string& label, const TasksSortBy& sort_by) = 0;
    /**
     * Sends request to shows all tasks expect subtasks
     * @param sort_by - enum which informs by what parameter sort requested tasks
     * @return \b ManyTasksWithId - protobuf message containing requesting tasks
     */
    virtual ManyTasksWithId ShowParents(const TasksSortBy& sort_by) = 0;
    /**
     * Sends request to shows task with given id with ith subtasks
     * @param id - protobuf message containing id of task to be shown
     * @param sort_by - enum which informs by what parameter sort children of requesting task
     * @return \b CompositeTask - protobuf message containing requesting task
     */
    virtual CompositeTask ShowTask(const TaskId& id, const TasksSortBy& sort_by) = 0;
    /**
     * Sends request to shows all tasks with subtasks
     * @param sort_by - enum which informs by what parameter sort requested tasks
     * @return \b ManyCompositeTasks - protobuf message containing requesting tasks
     */
    virtual ManyCompositeTasks ShowAll(const TasksSortBy& sort_by) = 0;

    /**
     * Sends request to saves state of model to given file
     * @param filename - string with name of file
     * @return \b bool - is saving successful
     */
    virtual bool Save(const std::string& filename) = 0;
    /**
     * Sends request to loads state of model from existing file
     * @param filename - string with name of file
     * @return \b bool - is loading successful
     */
    virtual bool Load(const std::string& filename) = 0;
};
}
