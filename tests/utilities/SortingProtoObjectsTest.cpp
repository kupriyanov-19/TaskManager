#include "gtest/gtest.h"
#include "utilities/SortingProtoObjects.h"
#include "utilities/ComparisonProtoObjects.h"
#include "utilities/CreateProtoObjects.h"

class SortingProtoObjectsTest : public ::testing::Test {
};

TEST_F(SortingProtoObjectsTest, ShouldSortById) {
    ManyTasksWithId tasks;
    tasks.mutable_tasks()->Add(CreateTaskWithId(CreateTaskId(2), CreateTask("first")));
    tasks.mutable_tasks()->Add(CreateTaskWithId(CreateTaskId(3), CreateTask("second")));
    tasks.mutable_tasks()->Add(CreateTaskWithId(CreateTaskId(1), CreateTask("third")));

    SortTasksWithId(tasks, TasksSortBy::ID);

    ASSERT_EQ(tasks.tasks_size(), 3);
    EXPECT_EQ(tasks.tasks()[0], CreateTaskWithId(CreateTaskId(1), CreateTask("third")));
    EXPECT_EQ(tasks.tasks()[1], CreateTaskWithId(CreateTaskId(2), CreateTask("first")));
    EXPECT_EQ(tasks.tasks()[2], CreateTaskWithId(CreateTaskId(3), CreateTask("second")));
}

TEST_F(SortingProtoObjectsTest, ShouldSortByPriority) {
    ManyTasksWithId tasks;
    tasks.mutable_tasks()->Add(CreateTaskWithId(CreateTaskId(1), CreateTask("", Task_Priority::Task_Priority_NONE)));
    tasks.mutable_tasks()->Add(CreateTaskWithId(CreateTaskId(2), CreateTask("", Task_Priority::Task_Priority_MEDIUM)));
    tasks.mutable_tasks()->Add(CreateTaskWithId(CreateTaskId(3), CreateTask("", Task_Priority::Task_Priority_HIGH)));

    SortTasksWithId(tasks, TasksSortBy::PRIORITY);

    ASSERT_EQ(tasks.tasks_size(), 3);
    EXPECT_EQ(tasks.tasks()[0], CreateTaskWithId(CreateTaskId(3), CreateTask("", Task_Priority::Task_Priority_HIGH)));
    EXPECT_EQ(tasks.tasks()[1], CreateTaskWithId(CreateTaskId(2), CreateTask("", Task_Priority::Task_Priority_MEDIUM)));
    EXPECT_EQ(tasks.tasks()[2], CreateTaskWithId(CreateTaskId(1), CreateTask("", Task_Priority::Task_Priority_NONE)));
}

TEST_F(SortingProtoObjectsTest, ShouldSortByDate) {
    ManyTasksWithId tasks;
    tasks.mutable_tasks()->Add(CreateTaskWithId(CreateTaskId(1), CreateTask("",
                                                                            Task_Priority::Task_Priority_NONE, 100)));
    tasks.mutable_tasks()->Add(CreateTaskWithId(CreateTaskId(2), CreateTask("",
                                                                            Task_Priority::Task_Priority_MEDIUM, 300)));
    tasks.mutable_tasks()->Add(CreateTaskWithId(CreateTaskId(3), CreateTask("",
                                                                            Task_Priority::Task_Priority_HIGH, 200)));

    SortTasksWithId(tasks, TasksSortBy::DAT);

    ASSERT_EQ(tasks.tasks_size(), 3);
    EXPECT_EQ(tasks.tasks()[0], CreateTaskWithId(CreateTaskId(1),CreateTask("",
                                                                            Task_Priority::Task_Priority_NONE, 100)));
    EXPECT_EQ(tasks.tasks()[1], CreateTaskWithId(CreateTaskId(3), CreateTask("",
                                                                             Task_Priority::Task_Priority_HIGH, 200)));
    EXPECT_EQ(tasks.tasks()[2], CreateTaskWithId(CreateTaskId(2), CreateTask("",
                                                                             Task_Priority::Task_Priority_MEDIUM, 300)));
}
