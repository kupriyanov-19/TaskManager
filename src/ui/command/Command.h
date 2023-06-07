#pragma once

#include <memory>
#include "Task.pb.h"
#include "Result.h"
#include "Error.h"
#include "ui/controller/Controller.h"

namespace ui::command {

class Command {
public:
    virtual Result execute(const std::shared_ptr<Controller>&) = 0;
    virtual ~Command() = default;
};

class Quit : public Command {
public:
    Result execute(const std::shared_ptr<Controller>&) override;
    ~Quit() override = default;
};

class Add : public Command {
public:
    explicit Add(const Task&);
    Result execute(const std::shared_ptr<Controller>&) override;
    ~Add() override = default;
private:
    const Task task_;
};

class AddSub : public Command {
public:
    explicit AddSub(const Task&, const TaskId&);
    Result execute(const std::shared_ptr<Controller>&) override;
    ~AddSub() override = default;
private:
    const Task task_;
    const TaskId parent_id_;
};

class Edit : public Command {
public:
    explicit Edit(const TaskId&, const Task&);
    Result execute(const std::shared_ptr<Controller>&) override;
    ~Edit() override = default;
private:
    const TaskId id_;
    const Task task_;
};

class Complete : public Command {
public:
    explicit Complete(const TaskId&);
    Result execute(const std::shared_ptr<Controller>&) override;
    ~Complete() override = default;
private:
    const TaskId id_;
};

class Delete : public Command {
public:
    explicit Delete(const TaskId&);
    Result execute(const std::shared_ptr<Controller>&) override;
    ~Delete() override = default;
private:
    const TaskId id_;
};

class Show : public Command {
public:
    explicit Show(bool, const TasksSortBy&);
    Result execute(const std::shared_ptr<Controller>&) override;
    ~Show() override = default;
private:
    const bool if_print_subtasks_;
    const TasksSortBy sort_by_;
};

class ShowTask : public Command {
public:
    explicit ShowTask(const TaskId&, const TasksSortBy&);
    Result execute(const std::shared_ptr<Controller>&) override;
    ~ShowTask() override = default;
private:
    const TaskId id_;
    const TasksSortBy sort_by_;
};

class ShowByLabel : public Command {
public:
    explicit ShowByLabel(const std::string&, const TasksSortBy&);
    Result execute(const std::shared_ptr<Controller>&) override;
    ~ShowByLabel() override = default;
private:
    const std::string label_;
    const TasksSortBy sort_by_;
};

class Save : public Command {
public:
    explicit Save(const std::string&);
    Result execute(const std::shared_ptr<Controller>&) override;
    ~Save() override = default;
private:
    const std::string filename_;
};

class Load : public Command {
public:
    explicit Load(const std::string&);
    Result execute(const std::shared_ptr<Controller>&) override;
    ~Load() override = default;
private:
    const std::string filename_;
};
}