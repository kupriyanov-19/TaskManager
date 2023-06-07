#pragma once

#include <ctime>
#include <string>
#include <optional>
#include "Task.pb.h"
#include "TaskStructures.pb.h"
#include "ui/step/Type.h"
#include "ui/command/Error.h"

namespace convert {
std::optional<Task::Priority> StringToPriority(const std::string&);
std::optional<time_t> StringToDate(const std::string&);
std::optional<ui::step::Type> StringToStepType(const std::string&);
std::optional<int> StringToId(const std::string&);
std::optional<TasksSortBy> StringToSortBy(const std::string&);
std::vector<std::string> StringToLabels(const std::string&);

std::string ToString(const Task::Priority&);
std::string ToString(const TasksSortBy&);
std::string ToString(const google::protobuf::Timestamp& date);
std::string ToString(const Task& task);
std::string ToString(const TaskWithId& task);
std::string ToString(const ui::command::Error&);
}

