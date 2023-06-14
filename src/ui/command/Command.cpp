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

    Result Efficien::execute(const std::shared_ptr<Controller>& controller) {
        auto tasks = controller ->ShowAll(TasksSortBy::ID);
        std::string res{""};
        int number=0, eff = 0;

        for(auto task: tasks.tasks()) {
            if (task.task().task().status() == Task_Status_COMPLETED) {
                number++;
                if (task.task().task().date().seconds()>=task.task().task().end().seconds()) eff++;
            }
             for(auto t : task.children()) {
                 if (t.task().status()==Task_Status_COMPLETED) {
                     number++;
                     if (t.task().date().seconds()>=t.task().end().seconds()) eff++;
                 }
             }
        }
        if (number==0) res+="You completed 100% (0/0) tasks on time";
        else {
            res += "You completed " + std::to_string(static_cast<int>(eff * 100 / number))
                   + "% (" + std::to_string(eff) + "/" + std::to_string(number) + ") "
                   + "of the tasks (consider subtasks) in this task set on time";
        }
        return Result(res);
    }

    Result Stat::execute(const std::shared_ptr<Controller>& controller) {
        auto tasks = controller ->ShowAll(TasksSortBy::ID);
        std::string res{""};
        if (tasks.tasks().size()==0) res+="You don't had any tasks";
        else res+="You completed:";

        for(auto task: tasks.tasks()) {
            res+="\nFrom task "+ task.task().task().title() + ": ";
            if (task.task().task().status() == Task_Status_COMPLETED) {
                res+="100%\n"; continue;
            }
            int number = 0; int eff = 0;
            for(auto t : task.children()) {
                if (t.task().status()==Task_Status_COMPLETED) {
                    number++; eff++;
                }
                else number++;
            }
            if (number == 0) res+="100%";
            else res += std::to_string(static_cast<int>(eff*100/number)) + "%";
        }
        return Result(res);
    }

}