#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "ModelService_mock.grpc.pb.h"
#include "ui/controller/GRPCEndPoint.h"
#include "utilities/CreateProtoObjects.h"
#include "utilities/ComparisonProtoObjects.h"
//#include "logging/Initialisation.h"

using namespace ui;
using ::testing::Return;
using ::testing::Invoke;
using ::testing::_;

class GRPCEndPointClientTest : public ::testing::Test {
public:
    void SetUp() override {
//        ConsoleLogging{boost::log::trivial::fatal};

        stub_ = std::make_shared<MockModelServiceStub>();
        end_point_ = std::make_shared<GRPCEndPoint>(stub_);

        task_ = std::make_shared<Task>();
        id_ = std::make_shared<TaskId>();
        many_tasks_with_id = std::make_shared<ManyTasksWithId>();
        composite_task_ = std::make_shared<CompositeTask>();
        many_composite_tasks_ = std::make_shared<ManyCompositeTasks>();
    }
protected:
    std::shared_ptr<MockModelServiceStub> stub_;
    std::shared_ptr<GRPCEndPoint> end_point_;

    std::shared_ptr<Task> task_;
    std::shared_ptr<TaskId> id_;
    std::shared_ptr<ManyTasksWithId> many_tasks_with_id;
    std::shared_ptr<CompositeTask> composite_task_;
    std::shared_ptr<ManyCompositeTasks> many_composite_tasks_;
};

TEST_F(GRPCEndPointClientTest, shouldAddTask) {
    auto add{[&](auto* context, auto& request, auto* response) {
        EXPECT_EQ(request.task(), *task_);
        response->set_result(true);
        return grpc::Status::OK;
    }};
    EXPECT_CALL(*stub_, AddTask(_, _, _))
            .WillOnce(Invoke(add));

    auto result = end_point_->AddTask(*task_);
    EXPECT_EQ(result, true);
}

TEST_F(GRPCEndPointClientTest, shouldHadleExceptionWhenAddTask) {
    auto add{[&](auto* context, auto& request, auto* response) {
        return grpc::Status::CANCELLED;
    }};
    EXPECT_CALL(*stub_, AddTask(_, _, _))
            .WillOnce(Invoke(add));

    auto result = end_point_->AddTask(*task_);
    EXPECT_EQ(result, false);
}

TEST_F(GRPCEndPointClientTest, shouldAddSubTask) {
    auto add_sub{[&](auto* context, auto& request, auto* response) {
        EXPECT_EQ(request.task(), *task_);
        EXPECT_EQ(request.parent_id(), *id_);
        response->set_result(false);
        return grpc::Status::OK;
    }};
    EXPECT_CALL(*stub_, AddSubTask(_, _, _))
            .WillOnce(Invoke(add_sub));

    auto result = end_point_->AddSubTask(*task_, *id_);
    EXPECT_EQ(result, false);
}

TEST_F(GRPCEndPointClientTest, shouldEditTask) {
    auto edit{[&](auto* context, auto& request, auto* response) {
        EXPECT_EQ(request.task(), *task_);
        EXPECT_EQ(request.id(), *id_);
        response->set_result(false);
        return grpc::Status::OK;
    }};
    EXPECT_CALL(*stub_, Edit(_, _, _))
            .WillOnce(Invoke(edit));

    auto result = end_point_->Edit(*id_, *task_);
    EXPECT_EQ(result, false);
}

TEST_F(GRPCEndPointClientTest, shouldCompleteTask) {
    auto complete{[&](auto* context, auto& request, auto* response) {
        EXPECT_EQ(request.id(), *id_);
        response->set_result(true);
        return grpc::Status::OK;
    }};
    EXPECT_CALL(*stub_, Complete(_, _, _))
            .WillOnce(Invoke(complete));

    auto result = end_point_->Complete(*id_);
    EXPECT_EQ(result, true);
}

TEST_F(GRPCEndPointClientTest, shouldDeleteTask) {
    auto delet{[&](auto* context, auto& request, auto* response) {
        EXPECT_EQ(request.id(), *id_);
        response->set_result(true);
        return grpc::Status::OK;
    }};
    EXPECT_CALL(*stub_, Delete(_, _, _))
            .WillOnce(Invoke(delet));

    auto result = end_point_->Delete(*id_);
    EXPECT_EQ(result, true);
}

TEST_F(GRPCEndPointClientTest, shouldShowByLabel) {
    auto show{[&](auto* context, auto& request, auto* response) {
        EXPECT_EQ(request.label(), "label");
        EXPECT_EQ(request.sort_by(), TasksSortBy::ID);
        response->set_allocated_tasks(new ManyTasksWithId(*many_tasks_with_id));
        return grpc::Status::OK;
    }};
    EXPECT_CALL(*stub_, ShowByLabel(_, _, _))
            .WillOnce(Invoke(show));

    auto result = end_point_->ShowByLabel("label", TasksSortBy::ID);
    EXPECT_EQ(result, *many_tasks_with_id);
}

TEST_F(GRPCEndPointClientTest, shouldHadleExceptionWhenShowByLabel) {
    auto show{[&](auto* context, auto& request, auto* response) {
        return grpc::Status::CANCELLED;
    }};
    EXPECT_CALL(*stub_, ShowByLabel(_, _, _))
            .WillOnce(Invoke(show));

    auto result = end_point_->ShowByLabel("label", TasksSortBy::ID);
    EXPECT_EQ(result.tasks_size(), 0);
}

TEST_F(GRPCEndPointClientTest, shouldShowParents) {
    auto show{[&](auto* context, auto& request, auto* response) {
        EXPECT_EQ(request.sort_by(), TasksSortBy::DAT);
        response->set_allocated_tasks(new ManyTasksWithId(*many_tasks_with_id));
        return grpc::Status::OK;
    }};
    EXPECT_CALL(*stub_, ShowParents(_, _, _))
            .WillOnce(Invoke(show));

    auto result = end_point_->ShowParents(TasksSortBy::DAT);
    EXPECT_EQ(result, *many_tasks_with_id);
}

TEST_F(GRPCEndPointClientTest, shouldShowTask) {
    auto show{[&](auto* context, auto& request, auto* response) {
        EXPECT_EQ(request.id(), *id_);
        EXPECT_EQ(request.sort_by(), TasksSortBy::ID);
        response->set_allocated_task(new CompositeTask(*composite_task_));
        return grpc::Status::OK;
    }};
    EXPECT_CALL(*stub_, ShowTask(_, _, _))
            .WillOnce(Invoke(show));

    auto result = end_point_->ShowTask(*id_, TasksSortBy::ID);
    EXPECT_EQ(result, *composite_task_);
}

TEST_F(GRPCEndPointClientTest, shouldShowAll) {
    auto show{[&](auto* context, auto& request, auto* response) {
        EXPECT_EQ(request.sort_by(), TasksSortBy::ID);
        response->set_allocated_tasks(new ManyCompositeTasks(*many_composite_tasks_));
        return grpc::Status::OK;
    }};
    EXPECT_CALL(*stub_, ShowAll(_, _, _))
            .WillOnce(Invoke(show));

    auto result = end_point_->ShowAll(TasksSortBy::ID);
    EXPECT_EQ(result, *many_composite_tasks_);
}

TEST_F(GRPCEndPointClientTest, shouldSave) {
    auto save{[&](auto* context, auto& request, auto* response) {
        EXPECT_EQ(request.filename(), "filename");
        response->set_result(false);
        return grpc::Status::OK;
    }};
    EXPECT_CALL(*stub_, Save(_, _, _))
            .WillOnce(Invoke(save));

    auto result = end_point_->Save("filename");
    EXPECT_EQ(result, false);
}

TEST_F(GRPCEndPointClientTest, shouldLoad) {
    auto save{[&](auto* context, auto& request, auto* response) {
        EXPECT_EQ(request.filename(), "filename");
        response->set_result(true);
        return grpc::Status::OK;
    }};
    EXPECT_CALL(*stub_, Load(_, _, _))
            .WillOnce(Invoke(save));

    auto result = end_point_->Load("filename");
    EXPECT_EQ(result, true);
}