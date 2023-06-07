#include "gtest/gtest.h"
#include "utilities/CreateProtoObjects.h"
#include "utilities/ComparisonProtoObjects.h"

class CreateProtoObjectsTest : public ::testing::Test {
};

TEST_F(CreateProtoObjectsTest, ShiuldCreateTaskId) {
    const int value{3};
    const TaskId id{CreateTaskId(value)};

    EXPECT_EQ(id.value(), value);
}

TEST_F(CreateProtoObjectsTest, ShouldCreateTask) {
    const std::string title{"first"};
    const Task_Priority priority{Task_Priority_MEDIUM};
    const time_t date{1000};
    const std::vector<std::string> labels{"label_1", "label_2"};
    const Task task{CreateTask(title, priority, date, labels)};

    EXPECT_EQ(task.title(), title);
    EXPECT_EQ(task.priority(), priority);
    EXPECT_EQ(task.date().seconds(), date);
    EXPECT_TRUE(std::equal(labels.begin(), labels.end(), task.labels().begin()));
    EXPECT_EQ(task.status(), Task_Status_IN_PROGRESS);
}

TEST_F(CreateProtoObjectsTest, ShouldCreateTaskWithId) {
    auto id{CreateTaskId(4)};
    auto task{CreateTask("first")};
    TaskWithId task_with_id{CreateTaskWithId(id, task)};

    ASSERT_TRUE(task_with_id.has_id());
    EXPECT_EQ(task_with_id.id(), id);
    ASSERT_TRUE(task_with_id.has_task());
    EXPECT_EQ(task_with_id.task(), task);
}

TEST_F(CreateProtoObjectsTest, ShouldCreateHierarchicalTask) {
    auto id{CreateTaskId(3)};
    auto task{CreateTask("second")};
    HierarchicalTask hierarchical_task{CreateHierarchicalTask(task, id)};

    ASSERT_TRUE(hierarchical_task.has_task());
    EXPECT_EQ(hierarchical_task.task(), task);
    ASSERT_TRUE(hierarchical_task.has_parent());
    EXPECT_EQ(hierarchical_task.parent(), id);
}

TEST_F(CreateProtoObjectsTest, ShouldCreateHierarchicalTaskWithoutParent) {
    auto task{CreateTask("third")};
    HierarchicalTask hierarchical_task{CreateHierarchicalTask(task, std::nullopt)};

    ASSERT_TRUE(hierarchical_task.has_task());
    EXPECT_EQ(hierarchical_task.task(), task);
    EXPECT_FALSE(hierarchical_task.has_parent());
}