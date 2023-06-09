#include "gtest/gtest.h"
#include "utilities/Convert.h"
#include "utilities/CreateProtoObjects.h"

class ConvertTest : public ::testing::Test {
};

TEST_F(ConvertTest, shouldConvertStringToPriority) {
    EXPECT_EQ(convert::StringToPriority("high"), Task_Priority_HIGH);
    EXPECT_EQ(convert::StringToPriority("medium"), Task_Priority_MEDIUM);
    EXPECT_EQ(convert::StringToPriority("low"), Task_Priority_LOW);
    EXPECT_EQ(convert::StringToPriority("none"), Task_Priority_NONE);
    EXPECT_EQ(convert::StringToPriority(""), Task_Priority_NONE);
    EXPECT_EQ(convert::StringToPriority("priority"), std::nullopt);
}

TEST_F(ConvertTest, shouldConvertStringToDate) {
    EXPECT_EQ(convert::StringToDate(""), 0);
    EXPECT_EQ(convert::StringToDate("23:00 11/11").value(), 61594808400);
    EXPECT_EQ(convert::StringToDate("11:00 3/5"), 61578172800);
    EXPECT_EQ(convert::StringToDate("10:12 7/7"), 61583785920);
    EXPECT_EQ(convert::StringToDate("4/4"), 61575627600);
    EXPECT_EQ(convert::StringToDate("5/11"), 61594207200);
    EXPECT_EQ(convert::StringToDate("25:00 3/5"), std::nullopt);
    EXPECT_EQ(convert::StringToDate("40/13"), std::nullopt);
}

TEST_F(ConvertTest, shouldConvertStringToStepType) {
    EXPECT_EQ(convert::StringToStepType("quit"), ui::step::Type::QUIT);
    EXPECT_EQ(convert::StringToStepType("help"), ui::step::Type::HELP);
    EXPECT_EQ(convert::StringToStepType("add"), ui::step::Type::ADD);
    EXPECT_EQ(convert::StringToStepType("add_subtask"), ui::step::Type::ADD_SUB);
    EXPECT_EQ(convert::StringToStepType("edit"), ui::step::Type::EDIT);
    EXPECT_EQ(convert::StringToStepType("complete"), ui::step::Type::COMPLETE);
    EXPECT_EQ(convert::StringToStepType("delete"), ui::step::Type::DELET);
    EXPECT_EQ(convert::StringToStepType("show"), ui::step::Type::SHOW);
    EXPECT_EQ(convert::StringToStepType("show_task"), ui::step::Type::SHOW_TASK);
    EXPECT_EQ(convert::StringToStepType("show_by_label"), ui::step::Type::SHOW_BY_LABEL);
    EXPECT_EQ(convert::StringToStepType("save"), ui::step::Type::SAVE);
    EXPECT_EQ(convert::StringToStepType("load"), ui::step::Type::LOAD);
    EXPECT_EQ(convert::StringToStepType("title"), std::nullopt);
    EXPECT_EQ(convert::StringToStepType("command"), std::nullopt);
    EXPECT_EQ(convert::StringToStepType(""), std::nullopt);
}

TEST_F(ConvertTest, shouldConvertStringToId) {
    EXPECT_EQ(convert::StringToId("523"), 523);
    EXPECT_EQ(convert::StringToId("0"), 0);
    EXPECT_EQ(convert::StringToId("1429"), 1429);
    EXPECT_EQ(convert::StringToId("fyg21"), std::nullopt);
    EXPECT_EQ(convert::StringToId("-65463"), std::nullopt);
    EXPECT_EQ(convert::StringToId(""), std::nullopt);
}

TEST_F(ConvertTest, shouldConvertStringToSortBy) {
    EXPECT_EQ(convert::StringToSortBy("id"), TasksSortBy::ID);
    EXPECT_EQ(convert::StringToSortBy("date"), TasksSortBy::DAT);
    EXPECT_EQ(convert::StringToSortBy("priority"), TasksSortBy::PRIORITY);
    EXPECT_EQ(convert::StringToSortBy(""), TasksSortBy::ID);
    EXPECT_EQ(convert::StringToSortBy("idd"), std::nullopt);
    EXPECT_EQ(convert::StringToSortBy("1"), std::nullopt);
}

TEST_F(ConvertTest, shouldConvertStringToVectorLabels) {
    const std::string labels = "first  second first q";
    std::vector<std::string> result{convert::StringToLabels(labels)};
    std::vector<std::string> expectation{"first", "second", "q"};
    ASSERT_EQ(result, expectation);
}

TEST_F(ConvertTest, shouldConvertPriorityToString) {
    EXPECT_EQ(convert::ToString(Task_Priority_HIGH), "high");
    EXPECT_EQ(convert::ToString(Task_Priority_MEDIUM), "medium");
    EXPECT_EQ(convert::ToString(Task_Priority_LOW), "low");
    EXPECT_EQ(convert::ToString(Task_Priority_NONE), "none");
}

TEST_F(ConvertTest, shouldConvertTasksSortByToString) {
    EXPECT_EQ(convert::ToString(TasksSortBy::ID), "id");
    EXPECT_EQ(convert::ToString(TasksSortBy::DAT), "date");
    EXPECT_EQ(convert::ToString(TasksSortBy::PRIORITY), "priority");
}

TEST_F(ConvertTest, shouldConvertDateToString) {
    google::protobuf::Timestamp date;
    date.set_seconds(0);
    EXPECT_EQ(convert::ToString(date), "none");
    date.set_seconds(61594808400);
    EXPECT_EQ(convert::ToString(date), "Nov 11 23:00");
    date.set_seconds(61578176400);
    EXPECT_EQ(convert::ToString(date), "May  3 12:00");
    date.set_seconds(61583789520);
    EXPECT_EQ(convert::ToString(date), "Jul  7 11:12");
    date.set_seconds(61575631200);
    EXPECT_EQ(convert::ToString(date), "Apr  4 01:00");
    date.set_seconds(61594207200);
    EXPECT_EQ(convert::ToString(date), "Nov  5 00:00");
}

TEST_F(ConvertTest, shouldConvertTaskToString) {
    EXPECT_EQ(convert::ToString(CreateTaskWithId(CreateTaskId(0), CreateTask("first"))),
              "id: 0, title: first, priority: none, date: none");
    EXPECT_EQ(convert::ToString(CreateTaskWithId(CreateTaskId(11), CreateTask("second", Task_Priority_LOW))),
              "id: 11, title: second, priority: low, date: none");
    EXPECT_EQ(convert::ToString(CreateTaskWithId(CreateTaskId(101),
                                                 CreateTask("third", Task_Priority_MEDIUM, 61594808400))),
              "id: 101, title: third, priority: medium, date: Nov 11 23:00");
    EXPECT_EQ(convert::ToString(CreateTaskWithId(CreateTaskId(2),
                                                 CreateTask("fourth", Task_Priority_NONE, 0, {"l1 l2"}))),
              "id: 2, title: fourth, priority: none, date: none, labels: l1 l2");
}

TEST_F(ConvertTest, shouldConvertErrorToString) {
    EXPECT_EQ(convert::ToString(ui::command::Error::INCORRECT_PARENT_ID),
              "Incorrect parent ID (for example, subtask cannot have child)");
    EXPECT_EQ(convert::ToString(ui::command::Error::NO_TASK_WITH_SUCH_ID),
              "There are no task with such ID");
    EXPECT_EQ(convert::ToString(ui::command::Error::CANNOT_SAVE_TO_FILE),
              "Cannot save to the specified file");
    EXPECT_EQ(convert::ToString(ui::command::Error::CANNOT_LOAD_FROM_FILE),
              "Cannot load from the specified file");

}