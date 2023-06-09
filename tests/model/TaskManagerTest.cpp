#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "model/TaskManager.h"
//#include "logging/Initialisation.h"

using ::testing::Return;

class IdGeneratorMock : public model::IdGenerator {
public:
    MOCK_METHOD(TaskId, GenerateId, (), (override));
};

class TaskManagerTest : public ::testing::Test {
public:
    void SetUp() override {
 //       ConsoleLogging{boost::log::trivial::fatal};

        generator_ = std::make_shared<IdGeneratorMock>();
        manager_ = std::make_shared<model::TaskManager>(generator_);
    }

protected:
    std::shared_ptr<IdGeneratorMock> generator_;
    std::shared_ptr<model::TaskManager> manager_;
};

TEST_F(TaskManagerTest, shouldAddTask) {
    EXPECT_CALL(*generator_, GenerateId())
            .Times(2)
            .WillOnce(Return(CreateTaskId(0)))
            .WillOnce(Return(CreateTaskId(1)));

    EXPECT_TRUE(manager_->AddTask(CreateTask("first")));
    EXPECT_EQ(manager_->ShowTask(CreateTaskId(0), TasksSortBy::ID).task().task(), CreateTask("first"));
    EXPECT_TRUE(manager_->AddTask(CreateTask("second")));
    EXPECT_EQ(manager_->ShowTask(CreateTaskId(1), TasksSortBy::ID).task().task(), CreateTask("second"));
}

TEST_F(TaskManagerTest, shouldReturnFalseIfIdGeneratorWorksdBadly) {
    EXPECT_CALL(*generator_, GenerateId())
            .Times(2)
            .WillOnce(Return(CreateTaskId(0)))
            .WillOnce(Return(CreateTaskId(0)));

    EXPECT_TRUE(manager_->AddTask(CreateTask("first")));
    EXPECT_FALSE(manager_->AddTask(CreateTask("first")));
}

TEST_F(TaskManagerTest, shouldAddSubTask) {
    EXPECT_CALL(*generator_, GenerateId())
            .Times(2)
            .WillOnce(Return(CreateTaskId(0)))
            .WillOnce(Return(CreateTaskId(1)));

    EXPECT_TRUE(manager_->AddTask(CreateTask("first")));
    EXPECT_TRUE(manager_->AddSubTask(CreateTask("second"), CreateTaskId(0)));
    EXPECT_EQ(manager_->ShowTask(CreateTaskId(1), TasksSortBy::ID).task().task(), CreateTask("second"));
}

TEST_F(TaskManagerTest, shouldReturnFalseIfParentsIdIsIncorrect) {
    EXPECT_CALL(*generator_, GenerateId())
            .Times(1)
            .WillOnce(Return(CreateTaskId(0)));

    EXPECT_TRUE(manager_->AddTask(CreateTask("first")));
    EXPECT_FALSE(manager_->AddSubTask(CreateTask("second"), CreateTaskId(3)));
}

TEST_F(TaskManagerTest, shouldReturnFalseIfAddSubtaskToSubtask) {
    EXPECT_CALL(*generator_, GenerateId())
            .Times(2)
            .WillOnce(Return(CreateTaskId(0)))
            .WillOnce(Return(CreateTaskId(1)));

    EXPECT_TRUE(manager_->AddTask(CreateTask("first")));
    EXPECT_TRUE(manager_->AddSubTask(CreateTask("second"), CreateTaskId(0)));
    EXPECT_FALSE(manager_->AddSubTask(CreateTask("third"), CreateTaskId(1)));
}

TEST_F(TaskManagerTest, shouldReturnFalseIfAddSubtaskToCompletedTask) {
    EXPECT_CALL(*generator_, GenerateId())
            .Times(1)
            .WillOnce(Return(CreateTaskId(0)));

    EXPECT_TRUE(manager_->AddTask(CreateTask("first")));
    EXPECT_TRUE(manager_->Complete(CreateTaskId(0)));
    EXPECT_FALSE(manager_->AddSubTask(CreateTask("second"), CreateTaskId(0)));
}

TEST_F(TaskManagerTest, shouldEditTask) {
    EXPECT_CALL(*generator_, GenerateId())
            .Times(2)
            .WillOnce(Return(CreateTaskId(0)))
            .WillOnce(Return(CreateTaskId(1)));

    EXPECT_TRUE(manager_->AddTask(CreateTask("first")));
    EXPECT_TRUE(manager_->AddSubTask(CreateTask("second"), CreateTaskId(0)));
    EXPECT_TRUE(manager_->Edit(CreateTaskId(0), CreateTask("third")));
    EXPECT_TRUE(manager_->Edit(CreateTaskId(1), CreateTask("fourth")));
    EXPECT_EQ(manager_->ShowTask(CreateTaskId(0), TasksSortBy::ID).task().task(), CreateTask("third"));
    EXPECT_EQ(manager_->ShowTask(CreateTaskId(1),TasksSortBy::ID).task().task(), CreateTask("fourth"));
}

TEST_F(TaskManagerTest, shouldReturnFalseWhenEditIfIDNotExist) {
    EXPECT_CALL(*generator_, GenerateId())
            .Times(1)
            .WillOnce(Return(CreateTaskId(0)));

    EXPECT_TRUE(manager_->AddTask(CreateTask("first")));
    EXPECT_FALSE(manager_->Edit(CreateTaskId(1), CreateTask("second")));
}

TEST_F(TaskManagerTest, shouldReturnFalseIfEditCompletedTask) {
    EXPECT_CALL(*generator_, GenerateId())
            .Times(1)
            .WillOnce(Return(CreateTaskId(0)));

    EXPECT_TRUE(manager_->AddTask(CreateTask("first")));
    EXPECT_TRUE(manager_->Complete(CreateTaskId(0)));
    EXPECT_FALSE(manager_->Edit(CreateTaskId(0), CreateTask("second")));
}

TEST_F(TaskManagerTest, shouldCompleteTask) {
    EXPECT_CALL(*generator_, GenerateId())
            .Times(2)
            .WillOnce(Return(CreateTaskId(0)))
            .WillOnce(Return(CreateTaskId(1)));

    EXPECT_TRUE(manager_->AddTask(CreateTask("first")));
    EXPECT_TRUE(manager_->AddSubTask(CreateTask("second"), CreateTaskId(0)));
    EXPECT_TRUE(manager_->Complete(CreateTaskId(0)));
    EXPECT_EQ(manager_->ShowAll(TasksSortBy::ID).tasks().size(), 1);
}

TEST_F(TaskManagerTest, shouldReturnFalseWhenCompleteIfIDNotExist) {
    EXPECT_CALL(*generator_, GenerateId())
            .Times(1)
            .WillOnce(Return(CreateTaskId(0)));

    EXPECT_TRUE(manager_->AddTask(CreateTask("first")));
    EXPECT_FALSE(manager_->Complete(CreateTaskId(1)));
}

TEST_F(TaskManagerTest, shouldDeleteTask) {
    EXPECT_CALL(*generator_, GenerateId())
            .Times(2)
            .WillOnce(Return(CreateTaskId(0)))
            .WillOnce(Return(CreateTaskId(1)));

    EXPECT_TRUE(manager_->AddTask(CreateTask("first")));
    EXPECT_TRUE(manager_->AddSubTask(CreateTask("second"), CreateTaskId(0)));
    EXPECT_TRUE(manager_->Delete(CreateTaskId(0)));
    EXPECT_EQ(manager_->ShowAll(TasksSortBy::ID).tasks().size(), 0);
}

TEST_F(TaskManagerTest, shouldReturnFalseWhenDeleteIfIDNotExist) {
    EXPECT_CALL(*generator_, GenerateId())
            .Times(1)
            .WillOnce(Return(CreateTaskId(0)));

    EXPECT_TRUE(manager_->AddTask(CreateTask("first")));
    EXPECT_FALSE(manager_->Delete(CreateTaskId(1)));
}

TEST_F(TaskManagerTest, shouldShowTaskWithSubtasks) {
    EXPECT_CALL(*generator_, GenerateId())
            .Times(3)
            .WillOnce(Return(CreateTaskId(0)))
            .WillOnce(Return(CreateTaskId(1)))
            .WillOnce(Return(CreateTaskId(2)));

    EXPECT_TRUE(manager_->AddTask(CreateTask("first")));
    EXPECT_TRUE(manager_->AddSubTask(CreateTask("second"), CreateTaskId(0)));
    EXPECT_TRUE(manager_->AddSubTask(CreateTask("third"), CreateTaskId(0)));

    CompositeTask result = manager_->ShowTask(CreateTaskId(0), TasksSortBy::ID);
    ASSERT_TRUE(result.has_task());
    EXPECT_EQ(result.task().task(), CreateTask("first"));
    ASSERT_EQ(result.children_size(), 2);
    EXPECT_EQ(result.children()[0].task(), CreateTask("second"));
    EXPECT_EQ(result.children()[1].task(), CreateTask("third"));
}

TEST_F(TaskManagerTest, shouldShowTasksByLabel) {
    EXPECT_CALL(*generator_, GenerateId())
            .Times(4)
            .WillOnce(Return(CreateTaskId(0)))
            .WillOnce(Return(CreateTaskId(1)))
            .WillOnce(Return(CreateTaskId(2)))
            .WillOnce(Return(CreateTaskId(3)));

    Task first(CreateTask("first", Task_Priority_LOW, 100, {"label_1", "label_2"}));
    Task second(CreateTask("second", Task_Priority_MEDIUM, 200, {"label_2", "label_3"}));
    Task third(CreateTask("third", Task_Priority_NONE, 300, {"label_1", "label_3"}));
    Task fourth(CreateTask("fourth", Task_Priority_HIGH, 500, {"label_2"}));
    ASSERT_TRUE(manager_->AddTask(first));
    ASSERT_TRUE(manager_->AddSubTask(second, CreateTaskId(0)));
    ASSERT_TRUE(manager_->AddTask(third));
    ASSERT_TRUE(manager_->AddSubTask(fourth, CreateTaskId(2)));

    ManyTasksWithId result = manager_->ShowByLabel("label_2", TasksSortBy::ID);
    ASSERT_EQ(result.tasks().size(), 3);
    EXPECT_EQ(result.tasks()[0].task(), first);
    EXPECT_EQ(result.tasks()[1].task(), second);
    EXPECT_EQ(result.tasks()[2].task(), fourth);
}

TEST_F(TaskManagerTest, shouldReturnNullptrIfIDNotExist) {
    EXPECT_CALL(*generator_, GenerateId())
            .WillOnce(Return(CreateTaskId(0)));

    EXPECT_TRUE(manager_->AddTask(CreateTask("first")));
    CompositeTask result = manager_->ShowTask(CreateTaskId(1), TasksSortBy::ID);
    ASSERT_FALSE(result.has_task());
}

TEST_F(TaskManagerTest, shouldShowAll) {
    EXPECT_CALL(*generator_, GenerateId())
            .Times(4)
            .WillOnce(Return(CreateTaskId(0)))
            .WillOnce(Return(CreateTaskId(1)))
            .WillOnce(Return(CreateTaskId(2)))
            .WillOnce(Return(CreateTaskId(3)));

    EXPECT_TRUE(manager_->AddTask(CreateTask("first")));
    EXPECT_TRUE(manager_->AddSubTask(CreateTask("second"),CreateTaskId(0)));
    EXPECT_TRUE(manager_->AddTask(CreateTask("third")));
    EXPECT_TRUE(manager_->AddSubTask(CreateTask("fourth"), CreateTaskId(2)));

    ManyCompositeTasks result = manager_->ShowAll(TasksSortBy::ID);
    ASSERT_EQ(result.tasks_size(), 2);
    EXPECT_EQ(result.tasks()[0].task().task(), CreateTask("first"));
    EXPECT_EQ(result.tasks()[0].children()[0].task(), CreateTask("second"));
    EXPECT_EQ(result.tasks()[1].task().task(), CreateTask("third"));
    EXPECT_EQ(result.tasks()[1].children()[0].task(), CreateTask("fourth"));
}

TEST_F(TaskManagerTest, shouldGetAllTasks) {
    EXPECT_CALL(*generator_, GenerateId())
            .Times(2)
            .WillOnce(Return(CreateTaskId(0)))
            .WillOnce(Return(CreateTaskId(1)));

    EXPECT_TRUE(manager_->AddTask(CreateTask("first")));
    EXPECT_TRUE(manager_->AddSubTask(CreateTask("second"), CreateTaskId(0)));

    model::ManyHierarchicalTasks result = manager_->GetAllTasks();
    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result[0].first, CreateTaskId(0));
    EXPECT_EQ(result[0].second, CreateHierarchicalTask(CreateTask("first"), std::nullopt));
    EXPECT_EQ(result[1].first, CreateTaskId(1));
    EXPECT_EQ(result[1].second, CreateHierarchicalTask(CreateTask("second"), CreateTaskId(0)));
}

TEST_F(TaskManagerTest, shouldRewriteTaskManager) {
    EXPECT_CALL(*generator_, GenerateId())
            .WillOnce(Return(CreateTaskId(0)));

    EXPECT_TRUE(manager_->AddTask(CreateTask("first")));

    model::ManyHierarchicalTasks tasks;
    tasks.emplace_back(CreateTaskId(1), CreateHierarchicalTask(CreateTask("first"), std::nullopt));
    tasks.emplace_back(CreateTaskId(2), CreateHierarchicalTask(CreateTask("second"), CreateTaskId(0)));
    manager_->Overwrite(tasks);

    model::ManyHierarchicalTasks result = manager_->GetAllTasks();
    EXPECT_EQ(result, tasks);
}