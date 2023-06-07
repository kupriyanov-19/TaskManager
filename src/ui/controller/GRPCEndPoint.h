#pragma once

#include <memory>
#include <utility>
#include <grpcpp/grpcpp.h>
#include "ModelService.grpc.pb.h"
#include "Controller.h"

namespace ui {
class GRPCEndPoint : public Controller {
public:
    explicit GRPCEndPoint(const std::shared_ptr<ModelService::StubInterface>& stub) : stub_{stub} {}

    bool AddTask(const Task& task) override;
    bool AddSubTask(const Task& task, const TaskId& id) override;
    bool Edit(const TaskId& id, const Task& task) override;
    bool Complete(const TaskId& id) override;
    bool Delete(const TaskId& id) override;

    ManyTasksWithId ShowByLabel(const std::string& label, const TasksSortBy&) override;
    ManyTasksWithId ShowParents(const TasksSortBy&) override;
    CompositeTask ShowTask(const TaskId& id, const TasksSortBy&) override;
    ManyCompositeTasks ShowAll(const TasksSortBy&) override;

    bool Save(const std::string& filename) override;
    bool Load(const std::string& filename) override;
private:
    std::shared_ptr<ModelService::StubInterface> stub_;
};
}
