#pragma once

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "ui/Context.h"

class ContextMock : public ui::Context {
public:
    explicit ContextMock(const std::shared_ptr<ui::command::Result>& result) : Context(result) {}
    explicit ContextMock(const std::string& name) : Context(name) {}

    MOCK_METHOD(std::shared_ptr<ui::command::Command>, command, (), (const, override));
    MOCK_METHOD(std::shared_ptr<ui::command::Result>, result, (), (const, override));
    MOCK_METHOD(std::shared_ptr<Task>, task, (), (const, override));
    MOCK_METHOD(std::string, wizard_string, (), (const, override));
    MOCK_METHOD(bool, if_finished, (), (const, override));
    MOCK_METHOD(void, set_command, (const std::shared_ptr<ui::command::Command>&), (override));
    MOCK_METHOD(void, set_result, (), (override));
    MOCK_METHOD(void, finished, (), (override));
};