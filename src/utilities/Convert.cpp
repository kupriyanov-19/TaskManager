#include "utilities/Convert.h"
#include <cassert>
#include <vector>
#include <time.h>
#include <sstream>
#include <iomanip>

namespace convert {

    std::optional<Task::Priority> StringToPriority(const std::string &priority) {
        if (priority == "high") return Task_Priority_HIGH;
        if (priority == "medium") return Task_Priority_MEDIUM;
        if (priority == "low") return Task_Priority_LOW;
        if (priority == "none" || priority.empty()) return Task_Priority_NONE;
        return std::nullopt;
    }

#ifdef _WIN32
    extern "C" char *strptime(const char *s, const char *f, struct tm *tm) {
        std::istringstream input(s);
        input.imbue(std::locale(setlocale(LC_ALL, nullptr)));
        input >> std::get_time(tm, f);
        if (input.fail()) {
            return nullptr;
        }
        return (char *) (s + input.tellg());
    }

    std::optional<time_t> StringToDate(const std::string &date) {
        if (date.empty())
            return 0;

        std::string pattern;
        if (date.size() > 5)
            pattern = "%H:%M %d/%m";
        else
            pattern = "%d/%m";

        tm time = {};
        if (!strptime(date.c_str(), pattern.c_str(), &time))
            return std::nullopt;

        time.tm_year = 123;
        tm first = {0};
        first.tm_year = 123;
        first.tm_mon = 1;
        first.tm_mday = 27;
        first.tm_hour = 3;
        time_t summer_time = mktime(&first);

        tm second = {};
        second.tm_year = 123;
        second.tm_mon = 8;
        second.tm_mday = 30;
        second.tm_hour = 3;
        time_t winter_time = mktime(&second);

        time_t result = mktime(&time);
        if (result >= summer_time && result < winter_time) result -= 3600;
        return result;
    }
#else
    std::optional<time_t> StringToDate(const std::string& date) {
    if (date.empty())
        return 0;

    std::string pattern;
    if (date.size() > 5)
        pattern = "%H:%M %d/%m";
    else
        pattern = "%d/%m";

    tm time = {};
    if (!strptime(date.c_str(), pattern.c_str(), &time))
        return std::nullopt;

    time.tm_year = 2021;
    tm first = {};
    first.tm_year = 2021;
    first.tm_mon = 2;
    first.tm_mday = 27;
    first.tm_hour = 3;
    time_t summer_time = mktime(&first);

    tm second = {};
    second.tm_year = 2021;
    second.tm_mon = 9;
    second.tm_mday = 30;
    second.tm_hour = 3;
    time_t winter_time = mktime(&second);

    time_t result = mktime(&time);
    if (result >= summer_time && result < winter_time) result -= 3600;
    return result;
}
#endif

    std::optional<ui::step::Type> StringToStepType(const std::string &command) {
        if (command == "quit") return ui::step::Type::QUIT;
        if (command == "help") return ui::step::Type::HELP;
        if (command == "add") return ui::step::Type::ADD;
        if (command == "add_subtask") return ui::step::Type::ADD_SUB;
        if (command == "edit") return ui::step::Type::EDIT;
        if (command == "complete") return ui::step::Type::COMPLETE;
        if (command == "delete") return ui::step::Type::DELET;
        if (command == "show") return ui::step::Type::SHOW;
        if (command == "show_task") return ui::step::Type::SHOW_TASK;
        if (command == "show_by_label") return ui::step::Type::SHOW_BY_LABEL;
        if (command == "save") return ui::step::Type::SAVE;
        if (command == "load") return ui::step::Type::LOAD;
        if (command == "create") return ui::step::Type::CREATE;
        if (command == "enter") return ui::step::Type::ENTER;
        if (command == "delete_space") return ui::step::Type::DELETE_SPACE;
        if (command == "efficiency") return ui::step::Type::EFF;
        if (command == "statistics") return ui::step::Type::STAT;
        return std::nullopt;
    }

    std::optional<int> StringToId(const std::string &id) {
        for (auto symbol: id)
            if (symbol < '0' || symbol > '9')
                return std::nullopt;
        try {
            int result = std::stoi(id);
            return result;
        }
        catch (...) {
            return std::nullopt;
        }
    }

    std::optional<TasksSortBy> StringToSortBy(const std::string &sort) {
        if (sort == "id" || sort.empty()) return TasksSortBy::ID;
        if (sort == "date") return TasksSortBy::DAT;
        if (sort == "priority") return TasksSortBy::PRIORITY;
        return std::nullopt;
    }

    std::vector<std::string> StringToLabels(const std::string &labels) {
        std::vector<std::string> result;
        std::string label;
        for (auto symbol: labels) {
            if (symbol == ' ') {
                if (!label.empty() && std::find(result.begin(), result.end(), label) == result.end())
                    result.push_back(label);
                label.clear();
            } else {
                label.push_back(symbol);
            }
        }
        if (!label.empty() && std::find(result.begin(), result.end(), label) == result.end())
            result.push_back(label);
        return result;
    }

    std::string ToString(const Task::Priority &priority) {
        switch (priority) {
            case Task_Priority_HIGH:
                return "high";
            case Task_Priority_MEDIUM:
                return "medium";
            case Task_Priority_LOW:
                return "low";
            default:
                return "none";
        }
    }

    std::string ToString(const TasksSortBy &sort_by) {
        switch (sort_by) {
            case ID:
                return "id";
            case PRIORITY:
                return "priority";
            case DAT:
                return "date";
            default:
                assert(false);
        }
    }

    std::string ToString(const google::protobuf::Timestamp &date) {
        if (date.seconds() == 0) return "none";
        time_t time{date.seconds()};
        std::string result = ctime(&time);
        for (int i = 0; i < 9; i++) result.pop_back();
        for (int i = 0; i < 4; i++) result.erase(result.begin());
        return result;
    }

    std::string ToString(const Task &task) {
        std::string result;
        result += "title: " + task.title();
        result += ", priority: " + ToString(task.priority());
        result += ", date: " + convert::ToString(task.date());
        if (!task.labels().empty()) {
            result += ", labels:";
            for (const auto &label: task.labels())
                result += " " + label;
        }
        return result;
    }

    std::string ToString(const TaskWithId &task) {
        std::string result;
        result += "id: " + std::to_string(task.id().value()) + ", ";
        result += ToString(task.task());
        return result;
    }

    std::string ToString(const ui::command::Error &error) {
        if (error == ui::command::Error::INCORRECT_PARENT_ID)
            return "Incorrect parent ID (for example, subtask cannot have child)";
        if (error == ui::command::Error::NO_TASK_WITH_SUCH_ID)
            return "There are no task with such ID";
        if (error == ui::command::Error::CANNOT_SAVE_TO_FILE)
            return "Cannot save to the specified file";
        if (error == ui::command::Error::CANNOT_LOAD_FROM_FILE)
            return "Cannot load from the specified file";
        assert(false);
    }
}