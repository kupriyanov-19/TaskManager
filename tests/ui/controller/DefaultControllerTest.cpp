#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "ui/controller/DefaultController.h"
#include "model/ModelMock.h"

using ::testing::Return;

class DefaultControllerTest : public ::testing::Test {
public:
    void SetUp() override {
        model_ = std::make_shared<ModelMock>(
                std::make_shared<model::TaskManager>(std::make_shared<model::IdGenerator>()));
        controller_ = std::make_shared<ui::DefaultController>(model_);
    }

protected:
    std::shared_ptr<ui::DefaultController> controller_;
    std::shared_ptr<ModelMock> model_;
};

TEST_F(DefaultControllerTest, shouldAddTask) {
    Task task{CreateTask("add")};
    EXPECT_CALL(*model_, AddTask(task)).WillOnce(Return(true));
    EXPECT_TRUE(controller_->AddTask(task));
}

TEST_F(DefaultControllerTest, shouldAddSubTask) {
    Task task{CreateTask("add_sub")};
    TaskId id{CreateTaskId(1)};
    EXPECT_CALL(*model_, AddSubTask(task, id)).WillOnce(Return(false));
    EXPECT_FALSE(controller_->AddSubTask(task, id));
}

TEST_F(DefaultControllerTest, shouldEditTask) {
    Task task{CreateTask("edit")};
    TaskId id{CreateTaskId(2)};
    EXPECT_CALL(*model_, Edit(id, task)).WillOnce(Return(true));
    EXPECT_TRUE(controller_->Edit(id, task));
}

TEST_F(DefaultControllerTest, shouldCompleteTask) {
    TaskId id{CreateTaskId(3)};
    EXPECT_CALL(*model_, Complete(id)).WillOnce(Return(true));
    EXPECT_TRUE(controller_->Complete(id));
}

TEST_F(DefaultControllerTest, shouldDeleteTask) {
    TaskId id{CreateTaskId(4)};
    EXPECT_CALL(*model_, Delete(id)).WillOnce(Return(false));
    EXPECT_FALSE(controller_->Delete(id));
}

TEST_F(DefaultControllerTest, shouldShowByLabel) {
    std::string label = "label";
    TasksSortBy sort_by = TasksSortBy::PRIORITY;
    ManyTasksWithId tasks;
    tasks.mutable_tasks()->Add(CreateTaskWithId(CreateTaskId(1), CreateTask("t")));

    EXPECT_CALL(*model_, ShowByLabel(label, sort_by)).WillOnce(Return(tasks));
    EXPECT_EQ(controller_->ShowByLabel(label, sort_by).tasks_size(), tasks.tasks_size());
}

TEST_F(DefaultControllerTest, shouldShowParents) {
    TasksSortBy sort_by = TasksSortBy::DAT;
    ManyTasksWithId tasks;
    tasks.mutable_tasks()->Add(CreateTaskWithId(CreateTaskId(1), CreateTask("t")));
    tasks.mutable_tasks()->Add(CreateTaskWithId(CreateTaskId(2), CreateTask("w")));

    EXPECT_CALL(*model_, ShowParents(sort_by)).WillOnce(Return(tasks));
    EXPECT_EQ(controller_->ShowParents(sort_by).tasks_size(), tasks.tasks_size());
}

TEST_F(DefaultControllerTest, shouldShowTask) {
    TaskId id{CreateTaskId(5)};
    TasksSortBy sort_by = TasksSortBy::ID;
    CompositeTask task;
    task.set_allocated_task(new TaskWithId(CreateTaskWithId(id, CreateTask("y"))));

    EXPECT_CALL(*model_, ShowTask(id, sort_by)).WillOnce(Return(task));

    auto result = controller_->ShowTask(id, sort_by);
    EXPECT_EQ(result.task().task(), task.task().task());
    EXPECT_EQ(result.task().id(), task.task().id());
}

TEST_F(DefaultControllerTest, shouldShowAllTasks) {
    TasksSortBy sort_by = TasksSortBy::DAT;
    ManyCompositeTasks tasks;

    EXPECT_CALL(*model_, ShowAll(sort_by)).WillOnce(Return(tasks));

    auto result = controller_->ShowAll(sort_by);
    EXPECT_EQ(result.tasks_size(), tasks.tasks_size());
}

TEST_F(DefaultControllerTest, shouldSaveToFile) {
    std::string filename = "file1";
    EXPECT_CALL(*model_, Save(filename)).WillOnce(Return(true));
    EXPECT_TRUE(controller_->Save(filename));
}

TEST_F(DefaultControllerTest, shouldLoadFromFile) {
    std::string filename = "file2";
    EXPECT_CALL(*model_, Load(filename)).WillOnce(Return(false));
    EXPECT_FALSE(controller_->Load(filename));
}