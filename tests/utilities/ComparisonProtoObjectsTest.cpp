#include "gtest/gtest.h"
#include "utilities/ComparisonProtoObjects.h"
#include "utilities/CreateProtoObjects.h"

class ComparisonProtoObjectsTest : public ::testing::Test {
};

TEST_F(ComparisonProtoObjectsTest, OperatorLessForTaskIdShouldWork) {
    TaskId id_1, id_2;

    id_1.set_value(0);
    id_2.set_value(1);
    EXPECT_LT(id_1, id_2);

    id_1.set_value(1000);
    id_2.set_value(200);
    EXPECT_LT(id_2, id_1);
}

TEST_F(ComparisonProtoObjectsTest, OperatorEqualsForTaskIdShouldWork) {
    TaskId id_1, id_2;

    id_1.set_value(5);
    id_2.set_value(5);
    EXPECT_TRUE(id_1 == id_2);

    id_1.set_value(700);
    id_2.set_value(701);
    EXPECT_FALSE(id_2 == id_1);
}

TEST_F(ComparisonProtoObjectsTest, OperatorEqualsForTaskShouldWork) {
    Task task_1, task_2;

    task_1.set_title("first");
    task_2.set_title("second");
    EXPECT_FALSE(task_1 == task_2);

    task_2.set_title("first");
    EXPECT_TRUE(task_1 == task_2);

    task_1.set_priority(Task_Priority_LOW);
    task_2.set_priority(Task_Priority_LOW);
    EXPECT_TRUE(task_1 == task_2);

    task_1.add_labels("label_1");
    task_1.add_labels("label_2");
    EXPECT_FALSE(task_1 == task_2);

    task_2.add_labels("label_1");
    EXPECT_FALSE(task_1 == task_2);
    task_2.add_labels("label_2");
    EXPECT_TRUE(task_1 == task_2);
}

TEST_F(ComparisonProtoObjectsTest, OperatorEqualsForTasksWithIdShouldWork) {
    EXPECT_TRUE(CreateTaskWithId(CreateTaskId(1), CreateTask("first")) ==
                 CreateTaskWithId(CreateTaskId(1), CreateTask("first")));
    EXPECT_FALSE(CreateTaskWithId(CreateTaskId(2), CreateTask("first")) ==
                CreateTaskWithId(CreateTaskId(1), CreateTask("first")));
    EXPECT_FALSE(CreateTaskWithId(CreateTaskId(1), CreateTask("first")) ==
                CreateTaskWithId(CreateTaskId(1), CreateTask("second")));
}

TEST_F(ComparisonProtoObjectsTest, OperatorEqualsForManyTasksWithIdShouldWork) {
    ManyTasksWithId tasks_1, tasks_2;

    tasks_1.mutable_tasks()->Add(CreateTaskWithId(CreateTaskId(1), CreateTask("first")));
    EXPECT_FALSE(tasks_1 == tasks_2);

    tasks_2.mutable_tasks()->Add(CreateTaskWithId(CreateTaskId(1), CreateTask("first")));
    EXPECT_TRUE(tasks_1 == tasks_2);

    tasks_1.mutable_tasks()->Add(CreateTaskWithId(CreateTaskId(2), CreateTask("first")));
    tasks_2.mutable_tasks()->Add(CreateTaskWithId(CreateTaskId(2), CreateTask("second")));
    EXPECT_FALSE(tasks_1 == tasks_2);
}

TEST_F(ComparisonProtoObjectsTest, OperatorEqualsForCompositeTaskShouldWork) {
    CompositeTask task_1, task_2;

    task_1.set_allocated_task(new TaskWithId(CreateTaskWithId(CreateTaskId(1), CreateTask("first"))));
    EXPECT_FALSE(task_1 == task_2);

    task_2.set_allocated_task(new TaskWithId(CreateTaskWithId(CreateTaskId(1), CreateTask("first"))));
    EXPECT_TRUE(task_1 == task_2);

    task_1.mutable_children()->Add(CreateTaskWithId(CreateTaskId(2), CreateTask("first")));
    task_2.mutable_children()->Add(CreateTaskWithId(CreateTaskId(1), CreateTask("first")));
    EXPECT_FALSE(task_1 == task_2);
}

TEST_F(ComparisonProtoObjectsTest, OperatorEqualsForManyCompositeTasksShouldWork) {
    ManyCompositeTasks tasks_1, tasks_2;
    CompositeTask task_1, task_2;
    task_1.set_allocated_task(new TaskWithId(CreateTaskWithId(CreateTaskId(1), CreateTask("first"))));
    task_2.set_allocated_task(new TaskWithId(CreateTaskWithId(CreateTaskId(1), CreateTask("first"))));

    tasks_1.mutable_tasks()->Add(std::move(task_1));
    EXPECT_FALSE(tasks_1 == tasks_2);

    tasks_2.mutable_tasks()->Add(std::move(task_2));
    EXPECT_TRUE(tasks_1 == tasks_2);

    task_1.mutable_children()->Add(CreateTaskWithId(CreateTaskId(2), CreateTask("first")));
    task_2.mutable_children()->Add(CreateTaskWithId(CreateTaskId(1), CreateTask("first")));
    tasks_1.mutable_tasks()->Add(std::move(task_1));
    tasks_2.mutable_tasks()->Add(std::move(task_2));
    EXPECT_FALSE(tasks_1 == tasks_2);
}

TEST_F(ComparisonProtoObjectsTest, OperatorEqualsForHierarchicalTaskShouldWork) {
    EXPECT_FALSE(CreateHierarchicalTask(CreateTask("first"), CreateTaskId(1)) ==
                 CreateHierarchicalTask(CreateTask("second"), CreateTaskId(1)));
    EXPECT_TRUE(CreateHierarchicalTask(CreateTask("second"), CreateTaskId(2)) ==
                CreateHierarchicalTask(CreateTask("second"), CreateTaskId(2)));
    EXPECT_FALSE(CreateHierarchicalTask(CreateTask("third"), std::nullopt) ==
                 CreateHierarchicalTask(CreateTask("third"), CreateTaskId(1)));
}