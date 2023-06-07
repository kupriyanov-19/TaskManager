#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "ui/view/View.h"
#include "utilities/CreateProtoObjects.h"
#include "utilities/ComparisonProtoObjects.h"

using ::testing::Return;

using namespace ui;

class ReaderMock : public ui::Reader {
public:
    MOCK_METHOD(std::string, ReadString, (), (override));
};

class PrinterMock : public ui::Printer {
public:
    MOCK_METHOD(void, PrintString, (const std::string &), (override));
};

class ViewTest : public ::testing::Test {
public:
    void SetUp() override {
        reader_ = std::make_shared<ReaderMock>();
        printer_ = std::make_shared<PrinterMock>();
        view_ = std::make_shared<View>(reader_, printer_);
    }

protected:
    std::shared_ptr<ReaderMock> reader_;
    std::shared_ptr<PrinterMock> printer_;
    std::shared_ptr<View> view_;
};

TEST_F(ViewTest, ShouldPrintHelp) {
    EXPECT_CALL(*printer_, PrintString(testing::_))
            .Times(1);
    view_->PrintHelp();
}

TEST_F(ViewTest, ShouldPrintQuit) {
    EXPECT_CALL(*printer_, PrintString("Good luck!\n"))
            .Times(1);
    view_->PrintQuit();
}

TEST_F(ViewTest, ShouldReadCommand) {
    EXPECT_CALL(*printer_, PrintString("> "))
            .Times(3);
    EXPECT_CALL(*printer_, PrintString("There is no such command\n"))
            .Times(2);
    EXPECT_CALL(*reader_, ReadString())
            .Times(3)
            .WillOnce(Return("q"))
            .WillOnce(Return("ADD"))
            .WillOnce(Return("add"));

    EXPECT_EQ(view_->ReadCommand(), step::Type::ADD);
}

TEST_F(ViewTest, ShouldReadId) {
    EXPECT_CALL(*printer_, PrintString(" ID: "))
            .Times(3);
    EXPECT_CALL(*printer_, PrintString("Enter the ID in the correct format\n"))
            .Times(2);
    EXPECT_CALL(*reader_, ReadString())
            .Times(3)
            .WillOnce(Return("1tgh"))
            .WillOnce(Return(""))
            .WillOnce(Return("3"));

    EXPECT_EQ(view_->ReadId(""), CreateTaskId(3));
}

TEST_F(ViewTest, ShouldReadParentId) {
    EXPECT_CALL(*printer_, PrintString(" Parent ID: "))
            .Times(3);
    EXPECT_CALL(*printer_, PrintString("Enter the ID in the correct format\n"))
            .Times(2);
    EXPECT_CALL(*reader_, ReadString())
            .Times(3)
            .WillOnce(Return("yfvguhbj"))
            .WillOnce(Return("3t4"))
            .WillOnce(Return("4"));

    EXPECT_EQ(view_->ReadParentId(""), CreateTaskId(4));
}

TEST_F(ViewTest, ShouldReadTitle) {
    EXPECT_CALL(*printer_, PrintString(" title: "))
            .Times(2);
    EXPECT_CALL(*printer_, PrintString("Title should be non-empty\n"))
            .Times(1);
    EXPECT_CALL(*reader_, ReadString())
            .Times(2)
            .WillOnce(Return(""))
            .WillOnce(Return("title"));

    EXPECT_EQ(view_->ReadTitle(""), "title");
}

TEST_F(ViewTest, ShouldReadPriority) {
    EXPECT_CALL(*printer_, PrintString(" priority (high, medium, low or none): "))
            .Times(3);
    EXPECT_CALL(*printer_, PrintString("There is no such priority\n"))
            .Times(2);
    EXPECT_CALL(*reader_, ReadString())
            .Times(3)
            .WillOnce(Return("priority"))
            .WillOnce(Return("qqq"))
            .WillOnce(Return("medium"));

    EXPECT_EQ(view_->ReadPriority(""), Task_Priority_MEDIUM);
}

TEST_F(ViewTest, ShouldReadDate) {
    EXPECT_CALL(*printer_, PrintString(" due date (in 12:12 12/12 or 12/12 format): "))
            .Times(3);
    EXPECT_CALL(*printer_, PrintString("Enter the date in the correct format (or don't enter anything):\n"))
            .Times(2);
    EXPECT_CALL(*reader_, ReadString())
            .Times(3)
            .WillOnce(Return("25:00 1/1"))
            .WillOnce(Return("15/13"))
            .WillOnce(Return("4/4"));

    EXPECT_EQ((view_->ReadDate("")).seconds(), 61575627600);
}

TEST_F(ViewTest, ShouldReadLabels) {
    EXPECT_CALL(*printer_, PrintString(" labels through a space (if there is no label, leave empty): "))
            .Times(1);

    EXPECT_CALL(*reader_, ReadString())
            .Times(1)
            .WillOnce(Return("first second first"));

    std::vector<std::string> labels{"first", "second"};
    EXPECT_EQ(view_->ReadLabels(""), labels);
}

TEST_F(ViewTest, ShouldReadConfirm) {
    EXPECT_CALL(*printer_, PrintString("Confirm? (y/n): "))
            .Times(3);

    EXPECT_CALL(*reader_, ReadString())
            .Times(3)
            .WillOnce(Return("qqq"))
            .WillOnce(Return(""))
            .WillOnce(Return("y"));

    EXPECT_EQ(view_->Confirm(), true);
}

TEST_F(ViewTest, ShouldReadIfPrintSubtasks) {
    EXPECT_CALL(*printer_, PrintString(" Print subtasks? (y/n): "))
            .Times(2);

    EXPECT_CALL(*reader_, ReadString())
            .Times(2)
            .WillOnce(Return("ww"))
            .WillOnce(Return("n"));

    EXPECT_EQ(view_->ReadIfPrintSubtasks(""), false);
}

TEST_F(ViewTest, ShouldReadLabel) {
    EXPECT_CALL(*printer_, PrintString(" label: "))
            .Times(2);
    EXPECT_CALL(*printer_, PrintString("Label should be non-empty\n"))
            .Times(1);
    EXPECT_CALL(*reader_, ReadString())
            .Times(2)
            .WillOnce(Return(""))
            .WillOnce(Return("label"));

    EXPECT_EQ(view_->ReadLabel(""), "label");
}

TEST_F(ViewTest, ShouldReadSortBy) {
    EXPECT_CALL(*printer_, PrintString(" sort by? (id, date or priority): "))
            .Times(2);

    EXPECT_CALL(*reader_, ReadString())
            .Times(2)
            .WillOnce(Return("idd"))
            .WillOnce(Return("priority"));

    EXPECT_EQ(view_->ReadSortBy(""), TasksSortBy::PRIORITY);
}

TEST_F(ViewTest, ShouldReadFilename) {
    EXPECT_CALL(*printer_, PrintString(" filename: "))
            .Times(2);
    EXPECT_CALL(*printer_, PrintString("Filename should be non-empty\n"))
            .Times(1);
    EXPECT_CALL(*reader_, ReadString())
            .Times(2)
            .WillOnce(Return(""))
            .WillOnce(Return("filename"));

    EXPECT_EQ(view_->ReadFilename(""), "filename");
}

TEST_F(ViewTest, ShouldPrintManyTasksWithId) {
    ManyTasksWithId tasks;
    tasks.mutable_tasks()->Add(CreateTaskWithId(CreateTaskId(0), CreateTask("first")));
    tasks.mutable_tasks()->Add(CreateTaskWithId(CreateTaskId(2), CreateTask("second")));

    EXPECT_CALL(*printer_, PrintString("id: 0, title: first, priority: none, date: none\n"))
            .Times(1);
    EXPECT_CALL(*printer_, PrintString("id: 2, title: second, priority: none, date: none\n"))
            .Times(1);

    view_->PrintManyTasksWithId(tasks);
}

TEST_F(ViewTest, ShouldPrintZeroTasksWithId) {
    ManyTasksWithId tasks;

    EXPECT_CALL(*printer_, PrintString("There are no such tasks now.\n"))
            .Times(1);

    view_->PrintManyTasksWithId(tasks);
}

TEST_F(ViewTest, ShouldPrintCompositeTask) {
    CompositeTask task;
    task.set_allocated_task(new TaskWithId(CreateTaskWithId(CreateTaskId(0), CreateTask("first"))));
    task.mutable_children()->Add(CreateTaskWithId(CreateTaskId(2), CreateTask("second")));
    task.mutable_children()->Add(CreateTaskWithId(CreateTaskId(4), CreateTask("third")));

    EXPECT_CALL(*printer_, PrintString("id: 0, title: first, priority: none, date: none  :\n"))
            .Times(1);
    EXPECT_CALL(*printer_, PrintString("   id: 2, title: second, priority: none, date: none\n"))
            .Times(1);
    EXPECT_CALL(*printer_, PrintString("   id: 4, title: third, priority: none, date: none\n"))
            .Times(1);

    view_->PrintCompositeTask(task);
}

TEST_F(ViewTest, ShouldPrintCompositeTaskWithoutChild) {
    CompositeTask task;
    task.set_allocated_task(new TaskWithId(CreateTaskWithId(CreateTaskId(0), CreateTask("first"))));

    EXPECT_CALL(*printer_, PrintString("id: 0, title: first, priority: none, date: none\n"))
            .Times(1);

    view_->PrintCompositeTask(task);
}

TEST_F(ViewTest, ShouldPrintManyCompositeTask) {
    CompositeTask task_1;
    task_1.set_allocated_task(new TaskWithId(CreateTaskWithId(CreateTaskId(0), CreateTask("first"))));
    CompositeTask task_2;
    task_2.set_allocated_task(new TaskWithId(CreateTaskWithId(CreateTaskId(2), CreateTask("second"))));
    task_2.mutable_children()->Add(CreateTaskWithId(CreateTaskId(4), CreateTask("third")));
    ManyCompositeTasks tasks;
    tasks.mutable_tasks()->Add(std::move(task_1));
    tasks.mutable_tasks()->Add(std::move(task_2));

    EXPECT_CALL(*printer_, PrintString("id: 0, title: first, priority: none, date: none\n"))
            .Times(1);
    EXPECT_CALL(*printer_, PrintString("id: 2, title: second, priority: none, date: none  :\n"))
            .Times(1);
    EXPECT_CALL(*printer_, PrintString("   id: 4, title: third, priority: none, date: none\n"))
            .Times(1);

    view_->PrintManyCompositeTasks(tasks);
}

TEST_F(ViewTest, ShouldPrintZeroCompositeTask) {
    ManyCompositeTasks tasks{};

    EXPECT_CALL(*printer_, PrintString("There are no such tasks now.\n"))
            .Times(1);

    view_->PrintManyCompositeTasks(tasks);
}

TEST_F(ViewTest, ShouldPrintError) {
    EXPECT_CALL(*printer_, PrintString("There are no task with such ID\n"))
            .Times(1);
    EXPECT_CALL(*printer_, PrintString("Incorrect parent ID (for example, subtask cannot have child)\n"))
            .Times(1);

    view_->PrintError(command::Error::NO_TASK_WITH_SUCH_ID);
    view_->PrintError(command::Error::INCORRECT_PARENT_ID);
}