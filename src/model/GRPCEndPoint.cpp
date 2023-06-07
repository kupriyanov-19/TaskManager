#include "GRPCEndPoint.h"
//#include "logging/Log.h"
#include "utilities/Convert.h"

namespace model {

grpc::Status GRPCEndPoint::AddTask(::grpc::ServerContext* context, const ::AddTaskRequest* request,
                                   ::AddTaskResponse* response) {
//    LOG(debug, "Request with Task: {" + request->task().ShortDebugString() + "} received");

    auto result = model_->AddTask(request->task());
    response->set_result(result);

//    LOG(debug, "Response " + static_cast<std::string>(
//            response->result() ? "{task successfully added}" : "{cannot add task}") + " sent");

    return grpc::Status::OK;
}

grpc::Status GRPCEndPoint::AddSubTask(::grpc::ServerContext* context, const ::AddSubTaskRequest* request,
                                      ::AddSubTaskResponse* response) {
    auto result = model_->AddSubTask(request->task(), request->parent_id());
    response->set_result(result);
    return grpc::Status::OK;
}

grpc::Status GRPCEndPoint::Edit(::grpc::ServerContext* context, const ::EditRequest* request,
                                ::EditResponse* response) {
    auto result = model_->Edit(request->id(), request->task());
    response->set_result(result);
    return grpc::Status::OK;
}

grpc::Status GRPCEndPoint::Delete(::grpc::ServerContext* context, const ::DeleteRequest* request,
                                  ::DeleteResponse* response) {
    auto result = model_->Delete(request->id());
    response->set_result(result);
    return grpc::Status::OK;
}

grpc::Status GRPCEndPoint::Complete(::grpc::ServerContext* context, const ::CompleteRequest* request,
                                    ::CompleteResponse* response) {
    auto result = model_->Complete(request->id());
    response->set_result(result);
    return grpc::Status::OK;
}

grpc::Status GRPCEndPoint::ShowByLabel(::grpc::ServerContext* context, const ::ShowByLabelRequest* request,
                                       ::ShowByLabelResponse* response) {
//    LOG(debug, "Request with label: {"
//            + request->label() + "} and TasksSortBy: {" + convert::ToString(request->sort_by()) +"} received");

    auto tasks = model_->ShowByLabel(request->label(), request->sort_by());
    response->set_allocated_tasks(new ManyTasksWithId(tasks));

//    LOG(debug, "Response from " + std::to_string(response->tasks().tasks_size())  + " tasks sent");

    return grpc::Status::OK;
}

grpc::Status GRPCEndPoint::ShowParents(::grpc::ServerContext* context, const ::ShowParentsRequest* request,
                                       ::ShowParentsResponse* response) {
    auto tasks = model_->ShowParents(request->sort_by());
    response->set_allocated_tasks(new ManyTasksWithId(tasks));
    return grpc::Status::OK;
}

grpc::Status GRPCEndPoint::ShowTask(::grpc::ServerContext* context, const ::ShowTaskRequest* request,
                                    ::ShowTaskResponse* response) {
    auto task = model_->ShowTask(request->id(), request->sort_by());
    response->set_allocated_task(new CompositeTask(task));
    return grpc::Status::OK;
}

grpc::Status GRPCEndPoint::ShowAll(::grpc::ServerContext* context, const ::ShowAllRequest* request,
                                   ::ShowAllResponse* response) {
    auto tasks = model_->ShowAll(request->sort_by());
    response->set_allocated_tasks(new ManyCompositeTasks(tasks));
    return grpc::Status::OK;
}

grpc::Status GRPCEndPoint::Save(::grpc::ServerContext* context,
                                const ::SaveRequest* request,
                                ::SaveResponse* response) {
    auto result = model_->Save(request->filename());
    response->set_result(result);
    return grpc::Status::OK;
}

grpc::Status GRPCEndPoint::Load(::grpc::ServerContext* context,
                                const ::LoadRequest* request,
                                ::LoadResponse* response) {
    auto result = model_->Load(request->filename());
    response->set_result(result);
    return grpc::Status::OK;
}
}