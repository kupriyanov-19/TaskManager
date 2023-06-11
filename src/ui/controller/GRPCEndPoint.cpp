#include "GRPCEndPoint.h"
#include "Requests.pb.h"
#include "Responses.pb.h"
#include "utilities/Convert.h"

namespace ui {

bool GRPCEndPoint::AddTask(const Task& task) {
    AddTaskRequest request;
    request.set_allocated_task(new Task(task));
    AddTaskResponse response;
    grpc::ClientContext context;

    grpc::Status status = stub_->AddTask(&context, request, &response);

    return response.result();
}

bool GRPCEndPoint::AddSubTask(const Task& task, const TaskId& parent_id) {
    AddSubTaskRequest request;
    request.set_allocated_task(new Task(task));
    request.set_allocated_parent_id(new TaskId(parent_id));
    AddSubTaskResponse response;
    grpc::ClientContext context;

    grpc::Status status = stub_->AddSubTask(&context, request, &response);
    return response.result();
}

bool GRPCEndPoint::Edit(const TaskId& id, const Task& task) {
    EditRequest request;
    request.set_allocated_task(new Task(task));
    request.set_allocated_id(new TaskId(id));
    EditResponse response;
    grpc::ClientContext context;

    grpc::Status status = stub_->Edit(&context, request, &response);
    return response.result();
}

bool GRPCEndPoint::Complete(const TaskId& id) {
    CompleteRequest request;
    request.set_allocated_id(new TaskId(id));
    CompleteResponse response;
    grpc::ClientContext context;

    grpc::Status status = stub_->Complete(&context, request, &response);
    return response.result();
}

bool GRPCEndPoint::Delete(const TaskId& id) {
    DeleteRequest request;
    request.set_allocated_id(new TaskId(id));
    DeleteResponse response;
    grpc::ClientContext context;

    grpc::Status status = stub_->Delete(&context, request, &response);
    return response.result();
}

ManyTasksWithId GRPCEndPoint::ShowByLabel(const std::string& label, const TasksSortBy& sort_by) {
    ShowByLabelRequest request;
    request.set_label(label);
    request.set_sort_by(sort_by);
    ShowByLabelResponse response;
    grpc::ClientContext context;

    grpc::Status status = stub_->ShowByLabel(&context, request, &response);

    return response.tasks();
}

ManyTasksWithId GRPCEndPoint::ShowParents(const TasksSortBy& sort_by) {
    ShowParentsRequest request;
    request.set_sort_by(sort_by);
    ShowParentsResponse response;
    grpc::ClientContext context;

    grpc::Status status = stub_->ShowParents(&context, request, &response);
    return response.tasks();
}

CompositeTask GRPCEndPoint::ShowTask(const TaskId& id, const TasksSortBy& sort_by) {
    ShowTaskRequest request;
    request.set_allocated_id(new TaskId(id));
    request.set_sort_by(sort_by);
    ShowTaskResponse response;
    grpc::ClientContext context;

    grpc::Status status = stub_->ShowTask(&context, request, &response);
    return response.task();
}

ManyCompositeTasks GRPCEndPoint::ShowAll(const TasksSortBy& sort_by) {
    ShowAllRequest request;
    request.set_sort_by(sort_by);
    ShowAllResponse response;
    grpc::ClientContext context;

    grpc::Status status = stub_->ShowAll(&context, request, &response);
    return response.tasks();
}

bool GRPCEndPoint::Save(const std::string& filename) {
    SaveRequest request;
    request.set_filename(filename);
    SaveResponse response;
    grpc::ClientContext context;

    grpc::Status status = stub_->Save(&context, request, &response);
    return response.result();
}

bool GRPCEndPoint::Load(const std::string& filename) {
    LoadRequest request;
    request.set_filename(filename);
    LoadResponse response;
    grpc::ClientContext context;

    grpc::Status status = stub_->Load(&context, request, &response);
    return response.result();
}

    bool GRPCEndPoint::Create(const std::string& name, const std::string& password) {
        CreateRequest request;
        request.set_name(name);
        CreateResponse response;
        grpc::ClientContext context;

        grpc::Status status = stub_->Create(&context, request, &response);
        return response.result();
    }

    bool GRPCEndPoint::Enter(const std::string& name, const std::string& password) {
        EnterRequest request;
        request.set_name(name);
        EnterResponse response;
        grpc::ClientContext context;

        grpc::Status status = stub_->Enter(&context, request, &response);
        return response.result();
    }

    bool GRPCEndPoint::DeleteSpace(const std::string& name, const std::string& password) {
        return true;
    }
}