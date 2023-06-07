#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "ui/controller/DefaultController.h"
#include "model/TaskManagerMock.h"

using ::testing::Return;

class ModelTest : public ::testing::Test {
public:
    void SetUp() override {
        manager_ = std::make_shared<TaskManagerMock>(std::make_shared<model::IdGenerator>());
        model_ = std::make_shared<model::Model>(manager_);
    }

protected:
    std::shared_ptr<TaskManagerMock> manager_;
    std::shared_ptr<model::Model> model_;
};

TEST_F(ModelTest, shouldAddTask) {
    Task task{CreateTask("add")};
    EXPECT_CALL(*manager_, AddTask(task)).WillOnce(Return(true));
    EXPECT_TRUE(model_->AddTask(task));
}

TEST_F(ModelTest, shouldAddSubTask) {
    Task task{CreateTask("add_sub")};
    TaskId id{CreateTaskId(1)};
    EXPECT_CALL(*manager_, AddSubTask(task, id)).WillOnce(Return(false));
    EXPECT_FALSE(model_->AddSubTask(task, id));
}

TEST_F(ModelTest, shouldEditTask) {
    Task task{CreateTask("edit")};
    TaskId id{CreateTaskId(2)};
    EXPECT_CALL(*manager_, Edit(id, task)).WillOnce(Return(true));
    EXPECT_TRUE(model_->Edit(id, task));
}

TEST_F(ModelTest, shouldCompleteTask) {
    TaskId id{CreateTaskId(3)};
    EXPECT_CALL(*manager_, Complete(id)).WillOnce(Return(true));
    EXPECT_TRUE(model_->Complete(id));
}

TEST_F(ModelTest, shouldDeleteTask) {
    TaskId id{CreateTaskId(4)};
    EXPECT_CALL(*manager_, Delete(id)).WillOnce(Return(false));
    EXPECT_FALSE(model_->Delete(id));
}

TEST_F(ModelTest, shouldShowByLabel) {
    std::string label = "label";
    TasksSortBy sort_by = TasksSortBy::PRIORITY;
    ManyTasksWithId tasks;
    tasks.mutable_tasks()->Add(CreateTaskWithId(CreateTaskId(1), CreateTask("t")));

    EXPECT_CALL(*manager_, ShowByLabel(label, sort_by)).WillOnce(Return(tasks));
    EXPECT_EQ(model_->ShowByLabel(label, sort_by).tasks_size(), tasks.tasks_size());
}

TEST_F(ModelTest, shouldShowParents) {
    TasksSortBy sort_by = TasksSortBy::DAT;
    ManyTasksWithId tasks;
    tasks.mutable_tasks()->Add(CreateTaskWithId(CreateTaskId(1), CreateTask("t")));
    tasks.mutable_tasks()->Add(CreateTaskWithId(CreateTaskId(2), CreateTask("w")));

    EXPECT_CALL(*manager_, ShowParents(sort_by)).WillOnce(Return(tasks));
    EXPECT_EQ(model_->ShowParents(sort_by).tasks_size(), tasks.tasks_size());
}

TEST_F(ModelTest, shouldShowTask) {
    TaskId id{CreateTaskId(5)};
    TasksSortBy sort_by = TasksSortBy::ID;
    CompositeTask task;
    task.set_allocated_task(new TaskWithId(CreateTaskWithId(id, CreateTask("y"))));

    EXPECT_CALL(*manager_, ShowTask(id, sort_by)).WillOnce(Return(task));

    auto result = model_->ShowTask(id, sort_by);
    EXPECT_EQ(result.task().task(), task.task().task());
    EXPECT_EQ(result.task().id(), task.task().id());
}

TEST_F(ModelTest, shouldShowAllTasks) {
    TasksSortBy sort_by = TasksSortBy::DAT;
    ManyCompositeTasks tasks;

    EXPECT_CALL(*manager_, ShowAll(sort_by)).WillOnce(Return(tasks));

    auto result = model_->ShowAll(sort_by);
    EXPECT_EQ(result.tasks_size(), tasks.tasks_size());
}

TEST_F(ModelTest, shouldWorkWithFile) {
    std::string filename = "model_test";
    ManyHierarchicalTasks tasks;
    tasks.emplace_back(CreateTaskId(3), CreateHierarchicalTask(CreateTask("f"), CreateTaskId(5)));

    EXPECT_CALL(*manager_, GetAllTasks()).WillOnce(Return(tasks));
    EXPECT_CALL(*manager_, Overwrite(testing::_)).Times(1);

    EXPECT_TRUE(model_->Save(filename));
    EXPECT_TRUE(model_->Load(filename));
    remove("model_test");
}
