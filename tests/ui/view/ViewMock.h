#pragma once

#include"gtest/gtest.h"
#include"gmock/gmock.h"
#include"ui/view/View.h"

class ViewMock : public ui::View {
public:
    ViewMock(const std::shared_ptr<ui::Reader>& reader, const std::shared_ptr<ui::Printer>& printer)
            : View(reader, printer) {}

    MOCK_METHOD(void, PrintHelp, (), (override));
    MOCK_METHOD(void, PrintQuit, (), (override));
    MOCK_METHOD(ui::step::Type, ReadCommand, (), (override));
    MOCK_METHOD(TaskId, ReadId, (const std::string&), (override));
    MOCK_METHOD(TaskId, ReadParentId, (const std::string&), (override));
    MOCK_METHOD(std::string, ReadTitle, (const std::string&), (override));
    MOCK_METHOD(Task::Priority, ReadPriority, (const std::string&), (override));
    MOCK_METHOD(google::protobuf::Timestamp, ReadDate, (const std::string&), (override));
    MOCK_METHOD(std::vector<std::string>, ReadLabels, (const std::string&), (override));
    MOCK_METHOD(bool, Confirm, (), (override));
    MOCK_METHOD(bool, ReadIfPrintSubtasks, (const std::string&), (override));
    MOCK_METHOD(std::string, ReadLabel, (const std::string&), (override));
    MOCK_METHOD(TasksSortBy, ReadSortBy, (const std::string&), (override));
    MOCK_METHOD(std::string, ReadFilename, (const std::string&), (override));

    MOCK_METHOD(void, PrintManyTasksWithId, (const ManyTasksWithId&), (override));
    MOCK_METHOD(void, PrintCompositeTask, (const CompositeTask&), (override));
    MOCK_METHOD(void, PrintManyCompositeTasks, (const ManyCompositeTasks&), (override));
    MOCK_METHOD(void, PrintError, (const ui::command::Error&), (override));
};