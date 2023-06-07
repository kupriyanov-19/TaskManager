#pragma once

#include "Task.pb.h"
#include "TaskStructures.pb.h"

bool operator<(const TaskId& first, const TaskId& second);

bool operator==(const TaskId& first, const TaskId& second);

bool operator==(const Task& first, const Task& second);

bool operator==(const TaskWithId& first, const TaskWithId& second);

bool operator==(const ManyTasksWithId& first, const ManyTasksWithId& second);

bool operator==(const CompositeTask& first, const CompositeTask& second);

bool operator==(const ManyCompositeTasks& first, const ManyCompositeTasks& second);

bool operator==(const HierarchicalTask& first, const HierarchicalTask& second);