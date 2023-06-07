#include "gtest/gtest.h"
#include "persistence/TaskPersister.h"
#include "utilities/ComparisonProtoObjects.h"
#include "utilities/CreateProtoObjects.h"

class TaskPersisterTest : public ::testing::Test {
};

TEST_F(TaskPersisterTest, shouldSaveLoadOneTask) {
    ManyHierarchicalTasks tasks;
    tasks.emplace_back(CreateTaskId(1), CreateHierarchicalTask(CreateTask("first"), std::nullopt));
    TaskPersister persister("persister_test_1");
    ASSERT_TRUE(persister.Save(tasks));

    std::optional<ManyHierarchicalTasks> result = persister.Load();
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), tasks);
    remove("persister_test_1");
}

TEST_F(TaskPersisterTest, shouldSaveLoadSeveralTasks) {
    ManyHierarchicalTasks tasks;
    tasks.emplace_back(CreateTaskId(3), CreateHierarchicalTask(CreateTask("first"), CreateTaskId(0)));
    tasks.emplace_back(CreateTaskId(4), CreateHierarchicalTask(CreateTask("second"), CreateTaskId(1)));
    tasks.emplace_back(CreateTaskId(5), CreateHierarchicalTask(CreateTask("third"), CreateTaskId(2)));
    TaskPersister persister("persister_test_2");
    ASSERT_TRUE(persister.Save(tasks));

    std::optional<ManyHierarchicalTasks> result = persister.Load();
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), tasks);
    remove("persister_test_2");
}

TEST_F(TaskPersisterTest, shouldSaveLoadTasksWithAndWithoutAParent) {
    ManyHierarchicalTasks tasks;
    tasks.emplace_back(CreateTaskId(1), CreateHierarchicalTask(CreateTask("first"), std::nullopt));
    tasks.emplace_back(CreateTaskId(2), CreateHierarchicalTask(CreateTask("second"), CreateTaskId(0)));
    TaskPersister persister("persister_test_3");
    ASSERT_TRUE(persister.Save(tasks));

    std::optional<ManyHierarchicalTasks> result;
    result = persister.Load();
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), tasks);
    remove("persister_test_3");
}