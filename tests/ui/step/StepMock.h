#pragma once

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "ui/step/Step.h"

class StepRootMock : public ui::step::Root {
public:
    StepRootMock(const std::shared_ptr<ui::Factory>& factory, const std::shared_ptr<ui::View>& view) :
            ui::step::Root{factory, view} {}
    MOCK_METHOD(std::shared_ptr<ui::step::Step>, execute, (ui::Context & ), (override));
};

class StepQuitMock : public ui::step::Quit {
public:
    StepQuitMock(const std::shared_ptr<ui::Factory>& factory, const std::shared_ptr<ui::View>& view) :
            ui::step::Quit{factory, view} {}
    MOCK_METHOD(std::shared_ptr<Step>, execute, (ui::Context & ), (override));
};

class StepHelpMock : public ui::step::Help {
public:
    StepHelpMock(const std::shared_ptr<ui::Factory>& factory, const std::shared_ptr<ui::View>& view) :
            ui::step::Help{factory, view} {}
    MOCK_METHOD(std::shared_ptr<Step>, execute, (ui::Context & ), (override));
};
