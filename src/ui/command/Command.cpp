#include "ui/command/Command.h"

namespace ui::command {

Result Quit::execute(const std::shared_ptr<Controller>&) {
    return Result(true);
}

Add::Add(const Task& task) : task_{task} {}

Result Add::execute(const std::shared_ptr<Controller>& controller) {
    controller->AddTask(task_);
    return Result(false);
}

AddSub::AddSub(const Task& task, const TaskId& parent)
        : task_{task}, parent_id_(parent) {}

Result AddSub::execute(const std::shared_ptr<Controller>& controller) {
    if (!controller->AddSubTask(task_, parent_id_))
        return Result(Error::INCORRECT_PARENT_ID);
    else
        return Result(false);
}

Edit::Edit(const TaskId& id, const Task& task) : id_{id}, task_{task} {}

Result Edit::execute(const std::shared_ptr<Controller>& controller) {
    if (!controller->Edit(id_, task_))
        return Result(Error::NO_TASK_WITH_SUCH_ID);
    else
        return Result(false);
}

Complete::Complete(const TaskId& id) : id_{id} {}

Result Complete::execute(const std::shared_ptr<Controller>& controller) {
    if (!controller->Complete(id_))
        return Result(Error::NO_TASK_WITH_SUCH_ID);
    else
        return Result(false);
}

Delete::Delete(const TaskId& id) : id_{id} {}

Result Delete::execute(const std::shared_ptr<Controller>& controller) {
    if (!controller->Delete(id_))
        return Result(Error::NO_TASK_WITH_SUCH_ID);
    else
        return Result(false);
}

Show::Show(bool if_print_subtasks, const TasksSortBy& sort_by)
        : if_print_subtasks_{if_print_subtasks}, sort_by_{sort_by} {}

Result Show::execute(const std::shared_ptr<Controller>& controller) {
    if (if_print_subtasks_)
        return Result(controller->ShowAll(sort_by_));
    else
        return Result(controller->ShowParents(sort_by_));
}

ShowTask::ShowTask(const TaskId& id, const TasksSortBy& sort_by)
        : id_{id}, sort_by_{sort_by} {}

Result ShowTask::execute(const std::shared_ptr<Controller>& controller) {
    if (auto result = controller->ShowTask(id_, sort_by_); result.has_task())
        return Result(result);
    else
        return Result(Error::NO_TASK_WITH_SUCH_ID);
}

ShowByLabel::ShowByLabel(const std::string& label, const TasksSortBy& sort_by)
        : label_{label}, sort_by_{sort_by} {}

Result ShowByLabel::execute(const std::shared_ptr<Controller>& controller) {
    return Result(controller->ShowByLabel(label_, sort_by_));
}

Save::Save(const std::string& filename) : filename_{filename} {}

Result Save::execute(const std::shared_ptr<Controller>& controller) {
    if (!controller->Save(filename_))
        return Result(Error::CANNOT_SAVE_TO_FILE);
    else
        return Result(false);
}

Load::Load(const std::string& filename) : filename_{filename} {}

Result Load::execute(const std::shared_ptr<Controller>& controller) {
    if (!controller->Load(filename_))
        return Result(Error::CANNOT_LOAD_FROM_FILE);
    else
        return Result(false);
}
}