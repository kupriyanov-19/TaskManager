#pragma once

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "model/Model.h"

class ModelMock : public model::Model {
public:
    explicit ModelMock(const std::shared_ptr<model::TaskManager>& manager) : model::Model(manager) {}

    MOCK_METHOD(bool, AddTask, (const Task&), (override));
    MOCK_METHOD(bool, AddSubTask, (const Task&, const TaskId&), (override));
    MOCK_METHOD(bool, Edit, (const TaskId&, const Task &), (override));
    MOCK_METHOD(bool, Complete, (const TaskId&), (override));
    MOCK_METHOD(bool, Delete, (const TaskId&), (override));
    MOCK_METHOD(ManyTasksWithId, ShowByLabel, (const std::string&, const TasksSortBy&), (override));
    MOCK_METHOD(ManyTasksWithId, ShowParents, (const TasksSortBy&), (override));
    MOCK_METHOD(CompositeTask, ShowTask, (const TaskId&, const TasksSortBy&), (override));
    MOCK_METHOD(ManyCompositeTasks, ShowAll, (const TasksSortBy&), (override));
    MOCK_METHOD(bool, Save, (const std::string&), (override));
    MOCK_METHOD(bool, Load, (const std::string&), (override));
};