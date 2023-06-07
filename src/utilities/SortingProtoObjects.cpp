#include "SortingProtoObjects.h"

bool ComparatorId(const TaskWithId& first, const TaskWithId& second) {
    return first.id() < second.id();
}

bool ComparatorPriority(const TaskWithId& first, const TaskWithId& second) {
    return static_cast<int>(first.task().priority()) < static_cast<int>(second.task().priority());
}

bool ComparatorDate(const TaskWithId& first, const TaskWithId& second) {
    return first.task().date().seconds() < second.task().date().seconds();
}

void SortTasksWithId(ManyTasksWithId& tasks, const TasksSortBy& sort_by) {
    if (sort_by == TasksSortBy::ID)
        std::sort(tasks.mutable_tasks()->begin(), tasks.mutable_tasks()->end(), ComparatorId);
    if (sort_by == TasksSortBy::PRIORITY)
        std::sort(tasks.mutable_tasks()->begin(), tasks.mutable_tasks()->end(), ComparatorPriority);
    if (sort_by == TasksSortBy::DAT)
        std::sort(tasks.mutable_tasks()->begin(), tasks.mutable_tasks()->end(), ComparatorDate);
}