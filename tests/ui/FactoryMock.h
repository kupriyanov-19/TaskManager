#pragma once

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "ui/Factory.h"

class FactoryMock : public ui::Factory {
public:
    explicit FactoryMock(const std::shared_ptr<ui::View>& view) : ui::Factory(view) {}

    MOCK_METHOD(std::shared_ptr<ui::step::Step>, CreateStep, (ui::step::Type), (override));
    MOCK_METHOD(std::shared_ptr<ui::step::Step>, GetInitialStep, (), (override));

    MOCK_METHOD(std::shared_ptr<ui::step::Step>, GetInitialSubStep, (), (override));
    MOCK_METHOD(std::shared_ptr<ui::step::Step>, GetNextSubStepFrom,
                (const ui::step::SubStepTitle &), (override));
    MOCK_METHOD(std::shared_ptr<ui::step::Step>, GetNextSubStepFrom,
                (const ui::step::SubStepPriority &), (override));
    MOCK_METHOD(std::shared_ptr<ui::step::Step>, GetNextSubStepFrom,
                (const ui::step::SubStepDate &), (override));
    MOCK_METHOD(std::shared_ptr<ui::step::Step>, GetNextSubStepFrom,
                (const ui::step::SubStepLabel &), (override));
};
