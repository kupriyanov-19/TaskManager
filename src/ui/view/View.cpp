#include "View.h"
#include "utilities/CreateProtoObjects.h"

namespace ui {

View::View(const std::shared_ptr<Reader>& reader, const std::shared_ptr<Printer>& printer)
        : reader_{reader}, printer_{printer} {}

void View::PrintGlobalHelp() {
    std::string output_string;
    output_string += "First of all, you need to enter an existing space of tasks or create a new one:\n";
    output_string += "You can use such command:\n";
    output_string += "create - Create new space of tasks\n";
    output_string += "enter - Enter an existing space of tasks\n";
    output_string += "delete - Delete an existing space of tasks\n";
    output_string += "end - If you want to finish work\n";
    printer_->PrintString(output_string);
}

void View::PrintHelp() {
    std::string output_string;
    output_string += "You can use such command:\n";
    output_string += "add - Add new task\n";
    output_string += "add_subtask - Add new subtask\n";
    output_string += "edit - Edit existent task\n";
    output_string += "complete - Complete existent task\n";
    output_string += "delete - Delete existent task\n";
    output_string += "show - Show all tasks\n";
    output_string += "show_task - Show task with its subtasks\n";
    output_string += "show_by_label - Show tasks with some specific label\n";
    output_string += "efficiency - What percentage of tasks you complete on time\n";
    output_string += "statistics - What percentage of tasks are completed for each composite task\n";
    output_string += "quit - Finish work with this task space\n\n";
    printer_->PrintString(output_string);
}

void View::PrintQuit() {
    printer_->PrintString("Good luck!\n");
}

step::Type View::ReadFirstCommand() {
    printer_->PrintString("> ");
    std::string command{reader_->ReadString()};

    if (command=="create") return step::Type::CREATE;
    if (command=="enter") return step::Type::ENTER;
    if (command=="delete") return step::Type::DELETE_SPACE;
    if (command=="help") return step::Type::HELP;
    if (command=="end") return step::Type::MAIN_QUIT;
    printer_->PrintString("There is no such command. You can use the command 'help' if you have any problems.\n");
    return ReadFirstCommand();
}

step::Type View::ReadCommand() {
    printer_->PrintString("> ");
    std::string command{reader_->ReadString()};

    if (auto result{convert::StringToStepType(command)}; result.has_value())
        return result.value();

    printer_->PrintString("There is no such command. You can use the command 'help' if you have any problems.\n");
    return ReadCommand();
}

TaskId View::ReadId(const std::string& wizard) {
    printer_->PrintString(wizard + " ID: ");
    std::string string_id{reader_->ReadString()};

    if (auto result{convert::StringToId(string_id)}; result.has_value())
        return CreateTaskId(result.value());

    printer_->PrintString("Enter the ID in the correct format\n");
    return ReadId(wizard);
}

TaskId View::ReadParentId(const std::string& wizard) {
    printer_->PrintString(wizard + " Parent ID: ");
    std::string parent_id{reader_->ReadString()};

    if (auto result{convert::StringToId(parent_id)}; result.has_value())
        return CreateTaskId(result.value());

    printer_->PrintString("Enter the ID in the correct format\n");
    return ReadParentId(wizard);
}

std::string View::ReadTitle(const std::string& wizard) {
    printer_->PrintString(wizard + " title: ");
    std::string title{reader_->ReadString()};

    if (!title.empty())
        return title;

    printer_->PrintString("Title should be non-empty\n");
    return ReadTitle(wizard);
}

Task::Priority View::ReadPriority(const std::string& wizard) {
    printer_->PrintString(wizard + " priority (high, medium, low or none): ");
    std::string priority{reader_->ReadString()};

    if (auto result{convert::StringToPriority(priority)}; result.has_value())
        return result.value();

    printer_->PrintString("There is no such priority\n");
    return ReadPriority(wizard);
}

google::protobuf::Timestamp View::ReadDate(const std::string& wizard) {
    printer_->PrintString(wizard + " due date (in 12:12 12/12 or 12/12 format): ");
    std::string time{reader_->ReadString()};

    if (auto result{convert::StringToDate(time)}; result.has_value()) {
        google::protobuf::Timestamp date;
        date.set_seconds(result.value());
        return date;
    }

    printer_->PrintString("Enter the date in the correct format (or don't enter anything):\n");
    return ReadDate(wizard);
}

std::vector<std::string> View::ReadLabels(const std::string& wizard) {
    printer_->PrintString(wizard + " labels through a space (if there is no label, leave empty): ");
    std::string labels{reader_->ReadString()};
    return convert::StringToLabels(labels);
}

bool View::Confirm() {
    printer_->PrintString("Confirm? (y/n): ");
    std::string answer{reader_->ReadString()};

    if (answer == "y") return true;
    if (answer == "n") return false;
    return Confirm();
}

bool View::ReadIfPrintSubtasks(const std::string& wizard) {
    printer_->PrintString(wizard + " Print subtasks? (y/n): ");
    std::string answer{reader_->ReadString()};

    if (answer == "y" || answer.empty()) return true;
    if (answer == "n") return false;
    return ReadIfPrintSubtasks(wizard);
}

std::string View::ReadLabel(const std::string& wizard) {
    printer_->PrintString(wizard + " label: ");
    std::string label{reader_->ReadString()};

    if (!label.empty())
        return label;

    printer_->PrintString("Label should be non-empty\n");
    return ReadLabel(wizard);
}

TasksSortBy View::ReadSortBy(const std::string& wizard) {
    printer_->PrintString(wizard + " sort by? (id, date or priority): ");
    std::string answer{reader_->ReadString()};

    if (auto result{convert::StringToSortBy(answer)}; result.has_value())
        return result.value();

    return ReadSortBy(wizard);
}

std::string View::ReadName(const std::string& wizard) {
    printer_->PrintString(wizard + " name: ");
    std::string filename{reader_->ReadString()};

    if (!filename.empty())
        return filename;

    printer_->PrintString("Name should be non-empty\n");
    return ReadName(wizard);
}

std::string View::ReadPassword(const std::string& wizard) {
    printer_->PrintString(wizard + " password: ");
    std::string password{reader_->ReadString()};
    return password;
}

void View::PrintManyTasksWithId(const ManyTasksWithId& tasks) {
    for (const auto& task: tasks.tasks())
        if (task.task().status()!=Task_Status_COMPLETED) {
            printer_->PrintString(convert::ToString(task) + '\n');
        }
    if (tasks.tasks().empty())
        printer_->PrintString("There are no such tasks now.\n");
}

void View::PrintCompositeTask(const CompositeTask& task) {
    if (task.task().task().status()==Task_Status_COMPLETED) return;
    std::string result = convert::ToString(task.task());
    if (!task.children().empty())
        result += "  :";
    printer_->PrintString(result + '\n');

    for (const auto& subtask: task.children()) {
        if (subtask.task().status()!=Task_Status_COMPLETED) {
            printer_->PrintString("   " + convert::ToString(subtask) + '\n');
        }
    }
}

void View::PrintManyCompositeTasks(const ManyCompositeTasks& tasks) {
    for (const auto& task: tasks.tasks())
        PrintCompositeTask(task);
    if (tasks.tasks().empty())
        printer_->PrintString("There are no such tasks now.\n");
}

void View::PrintError(const command::Error& error) {
    printer_->PrintString(convert::ToString(error) + '\n');
}
std::string View::ReadIP() {
    std::cout << "Where do you want to connect? Enter host and port in the format '0.0.0.0:1234'";
    std::string target_str;
    std::cin >> target_str;
    std::cin.ignore(3000, '\n');
    if (target_str == "default") target_str = "localhost:1234";
    if (target_str == "def") target_str = "192.168.0.104:1234";
    return target_str;
}

bool View::ReadCreateData(std::string& name, std::string& password) {
    const std::string name1{ReadName("[Create space of tasks]")};
    const std::string password1{ReadPassword("[Create space of tasks]")};
    const std::string password2{ReadPassword("[Create space of tasks] repeat")};
    if (password1!=password2) {
        PrintString("Passwords are not equivalent");
        return false;
    }
    else {
        if (Confirm()) {
            name = name1; password = password1;
            return true;
        }
        else return false;
    }
}

void View::PrintCreateResult(bool good) {
    if (good) PrintString("Space of tasks successfully created");
    else PrintString("This space of tasks already exists");
}

bool View::ReadEnterData(std::string&name, std::string&password) {
    const std::string name1{ReadName("[Enter space of tasks]")};
    const std::string password1{ReadPassword("[Enter space of tasks]")};
    name = name1; password = password1;
    return true;
}

void View::PrintEnterResult(bool good, std::string& name) {
    if (good) PrintString("You successfully entered the space of tasks: " + name);
    else PrintString("Wrong name or password");
}

bool View::ReadDeleteData(std::string&name, std::string&password) {
    const std::string name1{ReadName("[Delete space of tasks]")};
    const std::string password1{ReadPassword("[Delete space of tasks]")};
    name = name1; password = password1;
    return true;
}

void View::PrintDeleteResult(bool good, std::string& name) {
    if (good) PrintString("You successfully deleted the space of tasks: " + name);
    else PrintString("Wrong name or password");
}

void View::PrintCheckResult(bool good) {
    if (good) PrintString("You successfully connected to the server");
    else PrintString("This server is unavailable, please try again");
}
}