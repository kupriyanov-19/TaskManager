#pragma once

#include <algorithm>
#include "Task.pb.h"
#include "TaskStructures.pb.h"
#include "ComparisonProtoObjects.h"

void SortTasksWithId(ManyTasksWithId&, const TasksSortBy&);