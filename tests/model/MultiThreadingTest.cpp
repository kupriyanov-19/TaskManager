#include "gtest/gtest.h"
#include <thread>
#include "model/Model.h"

class MultiThreadingTest : public ::testing::Test {
public:
    void SetUp() override {
        model_ = std::make_shared<model::Model>(
                std::make_shared<model::TaskManager>(std::make_shared<model::IdGenerator>()));
        commands_counter_ = 0;
    }

    void ExecuteRandomCommand() {
        int type{rand() % 11};
        auto task{CreateTask("title")};
        auto id{CreateTaskId(rand() % 64)};
        auto sort_by{static_cast<TasksSortBy>(rand() % 3)};

        switch (type) {
            case 0:
                model_->AddTask(task);
                commands_counter_++;
                break;
            case 1:
                model_->AddSubTask(task, id);
                commands_counter_++;
                break;
            case 2:
                model_->Edit(id, task);
                commands_counter_++;
                break;
            case 3:
                model_->Complete(id);
                commands_counter_++;
                break;
            case 4:
                model_->Delete(id);
                commands_counter_++;
                break;
            case 5:
                model_->ShowByLabel("", sort_by);
                commands_counter_++;
                break;
            case 6:
                model_->ShowParents(sort_by);
                commands_counter_++;
                break;
            case 7:
                model_->ShowTask(id, sort_by);
                commands_counter_++;
                break;
            case 8:
                model_->ShowAll(sort_by);
                commands_counter_++;
                break;
            case 9:
                model_->Save("multi_threading_test");
                commands_counter_++;
                break;
            case 10:
                model_->Load("multi_threading_test");
                commands_counter_++;
                break;
        }
    }

    void Run(int number) {
        for (int i = 0; i < number; i++)
            ExecuteRandomCommand();
    }
protected:
    std::shared_ptr<model::Model> model_;
    std::atomic<int> commands_counter_;
};

TEST_F(MultiThreadingTest, shouldWorkWhenReceivesManyCommandsFromDifferentThreads) {
    int number_of_threads = 8;
    int number_of_commands = 1024;

    std::vector<std::thread> threads;
    for (int i = 0; i < number_of_threads; i++)
        threads.emplace_back(&MultiThreadingTest::Run, this, number_of_commands);

    for (auto& thread: threads)
        thread.join();

    remove("multi_threading_test");
    EXPECT_EQ(number_of_threads * number_of_commands, commands_counter_);
}