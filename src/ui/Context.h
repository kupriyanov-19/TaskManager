#pragma once

#include <memory>
#include <optional>
#include "Task.pb.h"
#include "command/Command.h"
#include "command/Result.h"

namespace ui {

class Context {
public:
    explicit Context(const std::shared_ptr<command::Result>& result) : result_{result} {}
    explicit Context(const std::string& name) : wizard_string_{name}, task_{new Task} {}
    virtual ~Context() = default;

    virtual std::shared_ptr<command::Command> command() const { return command_; }
    virtual std::shared_ptr<command::Result> result() const { return result_; }
    virtual std::shared_ptr<Task> task() const { return task_; }
    virtual std::string wizard_string() const {return wizard_string_;}
    virtual bool if_finished() const { return finished_; }

    virtual void set_command(const std::shared_ptr<command::Command>& command) {
        command_ = command;
        finished_ = true;
    }
    virtual void set_result() { result_ = std::make_shared<command::Result>(false); }
    virtual void finished() { finished_ = true; }
private:
    std::shared_ptr<command::Command> command_;
    std::shared_ptr<command::Result> result_;
    const std::shared_ptr<Task> task_;
    const std::string wizard_string_;
    bool finished_ = false;
};
}