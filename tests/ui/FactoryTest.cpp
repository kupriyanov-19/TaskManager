#include "gtest/gtest.h"
#include "ui/Factory.h"

using namespace ui;

class FactoryTest : public ::testing::Test {
public:
    void SetUp() override {
        factory_ = std::make_shared<Factory>(
                std::make_shared<View>(std::make_shared<ui::Reader>(), std::make_shared<ui::Printer>()));
    }
protected:
    std::shared_ptr<View> view_;
    std::shared_ptr<Factory> factory_;
};

TEST_F(FactoryTest, shouldCreateStep) {
    EXPECT_TRUE(std::dynamic_pointer_cast<step::Quit>(
            factory_->CreateStep(step::Type::QUIT)));
    EXPECT_TRUE(std::dynamic_pointer_cast<step::Help>(
            factory_->CreateStep(step::Type::HELP)));
    EXPECT_TRUE(std::dynamic_pointer_cast<step::Print>(
            factory_->CreateStep(step::Type::PRINT)));
    EXPECT_TRUE(std::dynamic_pointer_cast<step::Add>(
            factory_->CreateStep(step::Type::ADD)));
    EXPECT_TRUE(std::dynamic_pointer_cast<step::AddSub>(
            factory_->CreateStep(step::Type::ADD_SUB)));
    EXPECT_TRUE(std::dynamic_pointer_cast<step::Edit>(
            factory_->CreateStep(step::Type::EDIT)));
    EXPECT_TRUE(std::dynamic_pointer_cast<step::Complete>(
            factory_->CreateStep(step::Type::COMPLETE)));
    EXPECT_TRUE(std::dynamic_pointer_cast<step::Delete>(
            factory_->CreateStep(step::Type::DELET)));
    EXPECT_TRUE(std::dynamic_pointer_cast<step::Show>(
            factory_->CreateStep(step::Type::SHOW)));
    EXPECT_TRUE(std::dynamic_pointer_cast<step::ShowTask>(
            factory_->CreateStep(step::Type::SHOW_TASK)));
    EXPECT_TRUE(std::dynamic_pointer_cast<step::ShowByLabel>(
            factory_->CreateStep(step::Type::SHOW_BY_LABEL)));
    EXPECT_TRUE(std::dynamic_pointer_cast<step::Save>(
            factory_->CreateStep(step::Type::SAVE)));
    EXPECT_TRUE(std::dynamic_pointer_cast<step::Load>(
            factory_->CreateStep(step::Type::LOAD)));
}

TEST_F(FactoryTest, shouldReturnInitialStep) {
    EXPECT_TRUE(std::dynamic_pointer_cast<step::Root>(
            factory_->GetInitialStep()));
}

TEST_F(FactoryTest, shouldReturnInitialSubStep) {
    EXPECT_TRUE(std::dynamic_pointer_cast<step::SubStepTitle>(std::shared_ptr(
            factory_->GetInitialSubStep())));
}

TEST_F(FactoryTest, shouldReturnNextSubStepAfterTitle) {
    step::SubStepTitle step{factory_, view_};
    EXPECT_TRUE(std::dynamic_pointer_cast<step::SubStepPriority>(std::shared_ptr(
            factory_->GetNextSubStepFrom(step))));
}

TEST_F(FactoryTest, shouldReturnNextSubStepAfterPriprity) {
    step::SubStepPriority step{factory_, view_};
    EXPECT_TRUE(std::dynamic_pointer_cast<step::SubStepDate>(std::shared_ptr(
            factory_->GetNextSubStepFrom(step))));
}

TEST_F(FactoryTest, shouldReturnNextSubStepAfterDate) {
    step::SubStepDate step{factory_, view_};
    EXPECT_TRUE(std::dynamic_pointer_cast<step::SubStepLabel>(std::shared_ptr(
            factory_->GetNextSubStepFrom(step))));
}

TEST_F(FactoryTest, shouldReturnNextSubStepAfterLabel) {
    step::SubStepLabel step{factory_, view_};
    EXPECT_TRUE(std::dynamic_pointer_cast<step::SubStepLabel>(std::shared_ptr(
            factory_->GetNextSubStepFrom(step))));
}