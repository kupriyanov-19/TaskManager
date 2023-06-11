#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "ui/step/Step.h"
#include "ui/FactoryMock.h"
#include "ui/view/ViewMock.h"

using ::testing::Return;
using ::testing::_;

using namespace ui;
using namespace ui::step;

class StepTest : public ::testing::Test {
public:
    void SetUp() override {

        view_ = std::make_shared<ViewMock>(std::make_shared<Reader>(), std::make_shared<Printer>());
        factory_ = std::make_shared<FactoryMock>(view_);
    }

protected:
    std::shared_ptr<ViewMock> view_;
    std::shared_ptr<FactoryMock> factory_;
};

TEST_F(StepTest, shouldReadNextStep) {
    Root step{factory_, view_};
    Context context(std::make_shared<command::Result>(false));

    EXPECT_CALL(*view_, ReadCommand()).WillOnce(Return(Type::QUIT));
    EXPECT_CALL(*factory_, CreateStep(Type::QUIT)).Times(1);

    step.execute(context);
}

TEST_F(StepTest, shouldPrintResultIfNessesary) {
    Root step{factory_, view_};
    Context context{std::make_shared<command::Result>(command::Error::CANNOT_LOAD_FROM_FILE)};

    EXPECT_CALL(*factory_, CreateStep(Type::PRINT)).Times(1);

    step.execute(context);
}

TEST_F(StepTest, shouldQuit) {
    Quit step{factory_, view_};
    Context context{""};

//    EXPECT_CALL(*view_, PrintQuit()).Times(1);
    EXPECT_CALL(*factory_, GetInitialStep()).Times(1);

    step.execute(context);
    EXPECT_TRUE(std::dynamic_pointer_cast<command::Quit>(context.command()));
}

TEST_F(StepTest, shouldHelp) {
    Help step{factory_, view_};
    Context context{""};

    EXPECT_CALL(*view_, PrintHelp()).Times(1);
    EXPECT_CALL(*factory_, GetInitialStep()).Times(1);

    step.execute(context);
}

TEST_F(StepTest, shouldPrintError) {
    Print step{factory_, view_};
    Context context{std::make_shared<command::Result>(command::Error::INCORRECT_PARENT_ID)};

    EXPECT_CALL(*view_, PrintError(command::Error::INCORRECT_PARENT_ID)).Times(1);
    EXPECT_CALL(*factory_, GetInitialStep()).Times(1);

    step.execute(context);
    EXPECT_FALSE(context.result()->has_value());
}

TEST_F(StepTest, shouldPrintCompositeTask) {
    Print step{factory_, view_};
    Context context{std::make_shared<command::Result>(CompositeTask())};

    EXPECT_CALL(*view_, PrintCompositeTask(_)).Times(1);
    EXPECT_CALL(*factory_, GetInitialStep()).Times(1);

    step.execute(context);
    EXPECT_FALSE(context.result()->has_value());
}

TEST_F(StepTest, shouldPrintManyTasks) {
    Print step{factory_, view_};
    Context context{std::make_shared<command::Result>(ManyTasksWithId())};

    EXPECT_CALL(*view_, PrintManyTasksWithId(_)).Times(1);
    EXPECT_CALL(*factory_, GetInitialStep()).Times(1);

    step.execute(context);
    EXPECT_FALSE(context.result()->has_value());
}

TEST_F(StepTest, shouldPrintManyCompositeTasks) {
    Print step{factory_, view_};
    Context context{std::make_shared<command::Result>(ManyCompositeTasks())};

    EXPECT_CALL(*view_, PrintManyCompositeTasks(_)).Times(1);
    EXPECT_CALL(*factory_, GetInitialStep()).Times(1);

    step.execute(context);
    EXPECT_FALSE(context.result()->has_value());
}

TEST_F(StepTest, shouldCreateCommandComplete) {
    Complete step{factory_, view_};
    Context context{""};

    EXPECT_CALL(*view_, ReadId("[Complete Task]")).WillOnce(Return(TaskId()));
    EXPECT_CALL(*view_, Confirm()).WillOnce(Return(true));
    EXPECT_CALL(*factory_, GetInitialStep()).Times(1);

    step.execute(context);
    EXPECT_TRUE(std::dynamic_pointer_cast<command::Complete>(context.command()));
}

TEST_F(StepTest, shouldWorkIfNotConfirmComplete) {
    Complete step{factory_, view_};
    Context context{""};

    EXPECT_CALL(*view_, ReadId("[Complete Task]")).WillOnce(Return(TaskId()));
    EXPECT_CALL(*view_, Confirm()).WillOnce(Return(false));
    EXPECT_CALL(*factory_, GetInitialStep()).Times(1);

    step.execute(context);
    EXPECT_EQ(context.command(), nullptr);
}

TEST_F(StepTest, shouldCreateCommandDelete) {
    Delete step{factory_, view_};
    Context context{""};

    EXPECT_CALL(*view_, ReadId("[Delete Task]")).WillOnce(Return(TaskId()));
    EXPECT_CALL(*view_, Confirm()).WillOnce(Return(true));
    EXPECT_CALL(*factory_, GetInitialStep()).Times(1);

    step.execute(context);
    EXPECT_TRUE(std::dynamic_pointer_cast<command::Delete>(context.command()));
}

TEST_F(StepTest, shouldWorkIfNotConfirmDelete) {
    Delete step{factory_, view_};
    Context context{""};

    EXPECT_CALL(*view_, ReadId("[Delete Task]")).WillOnce(Return(TaskId()));
    EXPECT_CALL(*view_, Confirm()).WillOnce(Return(false));
    EXPECT_CALL(*factory_, GetInitialStep()).Times(1);

    step.execute(context);
    EXPECT_EQ(context.command(), nullptr);
}

TEST_F(StepTest, shouldCreateCommandSave) {
    Save step{factory_, view_};
    Context context{""};

    EXPECT_CALL(*view_, ReadName("[Save to file]")).WillOnce(Return(""));
    EXPECT_CALL(*view_, Confirm()).WillOnce(Return(true));
    EXPECT_CALL(*factory_, GetInitialStep()).Times(1);

    step.execute(context);
    EXPECT_TRUE(std::dynamic_pointer_cast<command::Save>(context.command()));
}

TEST_F(StepTest, shouldWorkIfNotConfirmSave) {
    Save step{factory_, view_};
    Context context{""};

    EXPECT_CALL(*view_, ReadName("[Save to file]")).WillOnce(Return(""));
    EXPECT_CALL(*view_, Confirm()).WillOnce(Return(false));
    EXPECT_CALL(*factory_, GetInitialStep()).Times(1);

    step.execute(context);
    EXPECT_EQ(context.command(), nullptr);
}

TEST_F(StepTest, shouldCreateCommandLoad) {
    Load step{factory_, view_};
    Context context{""};

    EXPECT_CALL(*view_, ReadName("[Load from file]")).WillOnce(Return(""));
    EXPECT_CALL(*view_, Confirm()).WillOnce(Return(true));
    EXPECT_CALL(*factory_, GetInitialStep()).Times(1);

    step.execute(context);
    EXPECT_TRUE(std::dynamic_pointer_cast<command::Load>(context.command()));
}

TEST_F(StepTest, shouldWorkIfNotConfirmLoad) {
    Load step{factory_, view_};
    Context context{""};

    EXPECT_CALL(*view_, ReadName("[Load from file]")).WillOnce(Return(""));
    EXPECT_CALL(*view_, Confirm()).WillOnce(Return(false));
    EXPECT_CALL(*factory_, GetInitialStep()).Times(1);

    step.execute(context);
    EXPECT_EQ(context.command(), nullptr);
}

TEST_F(StepTest, shouldCreateCommandShow) {
    Show step{factory_, view_};
    Context context{""};

    EXPECT_CALL(*view_, ReadIfPrintSubtasks("[Show]")).Times(1);
    EXPECT_CALL(*view_, ReadSortBy("[Show]")).Times(1);
    EXPECT_CALL(*factory_, GetInitialStep()).Times(1);

    step.execute(context);
    EXPECT_TRUE(std::dynamic_pointer_cast<command::Show>(context.command()));
}

TEST_F(StepTest, shouldCreateCommandShowTask) {
    ShowTask step{factory_, view_};
    Context context{""};

    EXPECT_CALL(*view_, ReadId("[Show Task]")).Times(1);
    EXPECT_CALL(*view_, ReadSortBy("[Show Task]")).Times(1);
    EXPECT_CALL(*factory_, GetInitialStep()).Times(1);

    step.execute(context);
    EXPECT_TRUE(std::dynamic_pointer_cast<command::ShowTask>(context.command()));
}

TEST_F(StepTest, shouldCreateCommandShowByLabel) {
    ShowByLabel step{factory_, view_};
    Context context{""};

    EXPECT_CALL(*view_, ReadLabel("[Show by label]")).Times(1);
    EXPECT_CALL(*view_, ReadSortBy("[Show by label]")).Times(1);
    EXPECT_CALL(*factory_, GetInitialStep()).Times(1);

    step.execute(context);
    EXPECT_TRUE(std::dynamic_pointer_cast<command::ShowByLabel>(context.command()));
}

TEST_F(StepTest, shouldAddTask) {
    Add step{factory_, view_};
    Context context{""};

    EXPECT_CALL(*factory_, GetInitialSubStep())
            .WillOnce(Return(std::make_shared<SubStepLabel>(std::make_shared<Factory>(view_), view_)));

    EXPECT_CALL(*view_, ReadLabels("[Add Task]")).WillOnce(Return(std::vector<std::string>{"label"}));
    EXPECT_CALL(*view_, Confirm()).WillOnce(Return(true));
    EXPECT_CALL(*factory_, GetInitialStep()).Times(1);

    step.execute(context);
    EXPECT_TRUE(std::dynamic_pointer_cast<command::Add>(context.command()));
}

TEST_F(StepTest, shouldAddSubTask) {
    AddSub step{factory_, view_};
    Context context{""};

    EXPECT_CALL(*view_, ReadParentId("[Add SubTask]")).WillOnce(Return(TaskId()));
    EXPECT_CALL(*factory_, GetInitialSubStep())
            .WillOnce(Return(std::make_shared<SubStepLabel>(std::make_shared<Factory>(view_), view_)));

    EXPECT_CALL(*view_, ReadLabels("[Add SubTask]")).WillOnce(Return(std::vector<std::string>{"label"}));
    EXPECT_CALL(*view_, Confirm()).WillOnce(Return(true));
    EXPECT_CALL(*factory_, GetInitialStep()).Times(1);

    step.execute(context);
    EXPECT_TRUE(std::dynamic_pointer_cast<command::AddSub>(context.command()));
}

TEST_F(StepTest, shouldEditTask) {
    Edit step{factory_, view_};
    Context context{""};

    EXPECT_CALL(*view_, ReadId("[Edit Task]")).WillOnce(Return(TaskId()));
    EXPECT_CALL(*factory_, GetInitialSubStep())
            .WillOnce(Return(std::make_shared<SubStepLabel>(std::make_shared<Factory>(view_), view_)));

    EXPECT_CALL(*view_, ReadLabels("[Edit Task]")).WillOnce(Return(std::vector<std::string>{"label"}));
    EXPECT_CALL(*view_, Confirm()).WillOnce(Return(true));
    EXPECT_CALL(*factory_, GetInitialStep()).Times(1);

    step.execute(context);
    EXPECT_TRUE(std::dynamic_pointer_cast<command::Edit>(context.command()));
}