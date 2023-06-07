#pragma once

#include <string>
#include <optional>
#include <vector>
#include "Task.pb.h"
#include "TaskStructures.pb.h"
#include "Error.h"

namespace ui::command {
struct Result {
public:
    explicit Result(bool i_finished) : finished{i_finished} {}
    explicit Result(Error i_error) : error{i_error}, finished{false} {}
    explicit Result(const ManyTasksWithId& tasks) : many_tasks{tasks}, finished{false} {}
    explicit Result(const CompositeTask& task) : composite_task{task}, finished{false} {}
    explicit Result(const ManyCompositeTasks& tasks) : many_composite_tasks{tasks}, finished{false} {}

    bool has_value() {
        return error.has_value() || many_tasks.has_value() || composite_task.has_value() ||
               many_composite_tasks.has_value();
    }

    const bool finished;
    const std::optional<Error> error;
    const std::optional<ManyTasksWithId> many_tasks;
    const std::optional<CompositeTask> composite_task;
    const std::optional<ManyCompositeTasks> many_composite_tasks;
};
}