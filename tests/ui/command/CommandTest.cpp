#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "ui/command/Command.h"
#include "ui/controller/DefaultControllerMock.h"
#include "utilities/CreateProtoObjects.h"

using ::testing::Return;

using namespace ui::command;

class CommandTest : public ::testing::Test {
public:
    void SetUp() override {
        task_ = std::make_shared<Task>(CreateTask("title"));
        id_ = std::make_shared<TaskId>(CreateTaskId(3));

        controller_ = std::make_shared<DefaultControllerMock>(std::make_shared<model::Model>(
                std::make_shared<model::TaskManager>(std::make_shared<model::IdGenerator>())));
    }

protected:
    std::shared_ptr<Task> task_;
    std::shared_ptr<TaskId> id_;

    std::shared_ptr<DefaultControllerMock> controller_;
};

TEST_F(CommandTest, shouldQuit) {
    auto command = std::make_shared<Quit>();
    Result result{command->execute(controller_)};
    EXPECT_TRUE(result.finished);
}

TEST_F(CommandTest, shouldAddTask) {
    auto command = std::make_shared<Add>(*task_);
    EXPECT_CALL(*controller_, AddTask(*task_))
            .WillOnce(Return(true));

    Result result{command->execute(controller_)};
    EXPECT_FALSE(result.finished);
    EXPECT_FALSE(result.has_value());
}

TEST_F(CommandTest, shouldAddSubTask) {
    auto command = std::make_shared<AddSub>(*task_, *id_);
    EXPECT_CALL(*controller_, AddSubTask(*task_, *id_))
            .WillOnce(Return(true));

    Result result{command->execute(controller_)};
    EXPECT_FALSE(result.finished);
    EXPECT_FALSE(result.has_value());
}

TEST_F(CommandTest, shouldHandleErrorWhenAddSubTask) {
    auto command = std::make_shared<AddSub>(*task_, *id_);
    EXPECT_CALL(*controller_, AddSubTask(*task_, *id_))
            .WillOnce(Return(false));

    Result result{command->execute(controller_)};
    EXPECT_FALSE(result.finished);
    ASSERT_TRUE(result.error.has_value());
    EXPECT_EQ(result.error.value(), Error::INCORRECT_PARENT_ID);
}

TEST_F(CommandTest, shouldEditTask) {
    auto command = std::make_shared<Edit>(*id_, *task_);
    EXPECT_CALL(*controller_, Edit(*id_, *task_))
            .WillOnce(Return(true));

    Result result{command->execute(controller_)};
    EXPECT_FALSE(result.finished);
    EXPECT_FALSE(result.has_value());
}

TEST_F(CommandTest, shouldHandleErrorWhenEditTask) {
    auto command = std::make_shared<Edit>(*id_, *task_);
    EXPECT_CALL(*controller_, Edit(*id_, *task_))
            .WillOnce(Return(false));

    Result result{command->execute(controller_)};
    EXPECT_FALSE(result.finished);
    ASSERT_TRUE(result.error.has_value());
    EXPECT_EQ(result.error.value(), Error::NO_TASK_WITH_SUCH_ID);
}

TEST_F(CommandTest, shouldCompleteTask) {
    auto command = std::make_shared<Complete>(*id_);
    EXPECT_CALL(*controller_, Complete(* id_))
            .WillOnce(Return(true));

    Result result{command->execute(controller_)};
    EXPECT_FALSE(result.finished);
    EXPECT_FALSE(result.has_value());
}

TEST_F(CommandTest, shouldHandleErrorWhenCompletetTask) {
    auto command = std::make_shared<Complete>(*id_);
    EXPECT_CALL(*controller_, Complete(* id_))
            .WillOnce(Return(false));

    Result result{command->execute(controller_)};
    EXPECT_FALSE(result.finished);
    ASSERT_TRUE(result.error.has_value());
    EXPECT_EQ(result.error.value(), Error::NO_TASK_WITH_SUCH_ID);
}

TEST_F(CommandTest, shouldDeleteTask) {
    auto command = std::make_shared<Delete>(*id_);
    EXPECT_CALL(*controller_, Delete(* id_))
            .WillOnce(Return(true));

    Result result{command->execute(controller_)};
    EXPECT_FALSE(result.finished);
    EXPECT_FALSE(result.has_value());
}

TEST_F(CommandTest, shouldHandleErrorWhenDeleteTask) {
    auto command = std::make_shared<Delete>(*id_);
    EXPECT_CALL(*controller_, Delete(* id_))
            .WillOnce(Return(false));

    Result result{command->execute(controller_)};
    EXPECT_FALSE(result.finished);
    ASSERT_TRUE(result.error.has_value());
    EXPECT_EQ(result.error.value(), Error::NO_TASK_WITH_SUCH_ID);
}

TEST_F(CommandTest, shouldShowAllTasksWithSubtasks) {
    ManyCompositeTasks tasks;

    auto command = std::make_shared<Show>(true, TasksSortBy::ID);
    EXPECT_CALL(*controller_, ShowAll(TasksSortBy::ID))
            .WillOnce(Return(tasks));

    Result result{command->execute(controller_)};
    EXPECT_FALSE(result.finished);
    ASSERT_TRUE(result.many_composite_tasks.has_value());
    EXPECT_EQ(result.many_composite_tasks.value(), tasks);
}

TEST_F(CommandTest, shouldShowOnlyParents) {
    ManyTasksWithId tasks;
    tasks.mutable_tasks()->Add(CreateTaskWithId(*id_, *task_));

    auto command = std::make_shared<Show>(false, TasksSortBy::PRIORITY);
    EXPECT_CALL(*controller_, ShowParents(TasksSortBy::PRIORITY))
            .WillOnce(Return(tasks));

    Result result{command->execute(controller_)};
    EXPECT_FALSE(result.finished);
    ASSERT_TRUE(result.many_tasks.has_value());
    EXPECT_EQ(result.many_tasks.value(), tasks);
}

TEST_F(CommandTest, shouldShowSomeTask) {
    CompositeTask task;
    task.set_allocated_task(new TaskWithId(CreateTaskWithId(*id_, *task_)));

    auto command = std::make_shared<ShowTask>(*id_, TasksSortBy::DAT);
    EXPECT_CALL(*controller_, ShowTask(*id_, TasksSortBy::DAT))
            .WillOnce(Return(task));

    Result result{command->execute(controller_)};
    EXPECT_FALSE(result.finished);
    ASSERT_TRUE(result.composite_task.has_value());
    EXPECT_EQ(result.composite_task.value(), task);
}

TEST_F(CommandTest, shouldHandleErrorWhenShowSomeTask) {
    auto command = std::make_shared<ShowTask>(*id_, TasksSortBy::ID);
    EXPECT_CALL(*controller_, ShowTask(*id_, TasksSortBy::ID))
            .WillOnce(Return(CompositeTask()));

    Result result{command->execute(controller_)};
    EXPECT_FALSE(result.finished);
    ASSERT_TRUE(result.error.has_value());
    EXPECT_EQ(result.error.value(), Error::NO_TASK_WITH_SUCH_ID);
}

TEST_F(CommandTest, shouldShowByLabel) {
    ManyTasksWithId tasks;
    tasks.mutable_tasks()->Add(CreateTaskWithId(*id_, *task_));

    auto command = std::make_shared<ShowByLabel>("label", TasksSortBy::PRIORITY);
    EXPECT_CALL(*controller_, ShowByLabel("label", TasksSortBy::PRIORITY))
            .WillOnce(Return(tasks));

    Result result{command->execute(controller_)};
    EXPECT_FALSE(result.finished);
    ASSERT_TRUE(result.many_tasks.has_value());
    EXPECT_EQ(result.many_tasks.value(), tasks);
}

TEST_F(CommandTest, shouldSaveToFile) {
    auto command = std::make_shared<Save>("filename");
    EXPECT_CALL(*controller_, Save("filename"))
            .WillOnce(Return(true));

    Result result{command->execute(controller_)};
    EXPECT_FALSE(result.finished);
    EXPECT_FALSE(result.has_value());
}

TEST_F(CommandTest, shouldHandleErrorWhenSaveToFile) {
    auto command = std::make_shared<Save>("filename");
    EXPECT_CALL(*controller_, Save("filename"))
            .WillOnce(Return(false));

    Result result{command->execute(controller_)};
    EXPECT_FALSE(result.finished);
    ASSERT_TRUE(result.error.has_value());
    EXPECT_EQ(result.error.value(), Error::CANNOT_SAVE_TO_FILE);
}

TEST_F(CommandTest, shouldLoadFromFile) {
    auto command = std::make_shared<Load>("filename");
    EXPECT_CALL(*controller_, Load("filename"))
            .WillOnce(Return(true));

    Result result{command->execute(controller_)};
    EXPECT_FALSE(result.finished);
    EXPECT_FALSE(result.has_value());
}

TEST_F(CommandTest, shouldHandleErrorWhenLoadFromFile) {
    auto command = std::make_shared<Load>("filename");
    EXPECT_CALL(*controller_, Load("filename"))
            .WillOnce(Return(false));

    Result result{command->execute(controller_)};
    EXPECT_FALSE(result.finished);
    ASSERT_TRUE(result.error.has_value());
    EXPECT_EQ(result.error.value(), Error::CANNOT_LOAD_FROM_FILE);
}