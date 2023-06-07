#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include "Requests.pb.h"
#include "Responses.pb.h"
#include "ModelService.grpc.pb.h"
#include "Model.h"

namespace model {
class GRPCEndPoint : public ModelService::Service {
public:
    explicit GRPCEndPoint(const std::shared_ptr<Model>& model) : model_{model} {}

    grpc::Status AddTask(::grpc::ServerContext* context, const ::AddTaskRequest* request,
                         ::AddTaskResponse* response) override;
    grpc::Status AddSubTask(::grpc::ServerContext* context, const ::AddSubTaskRequest* request,
                            ::AddSubTaskResponse* response) override;
    grpc::Status Edit(::grpc::ServerContext* context, const ::EditRequest* request,
                      ::EditResponse* response) override;
    grpc::Status Delete(::grpc::ServerContext* context, const ::DeleteRequest* request,
                        ::DeleteResponse* response) override;
    grpc::Status Complete(::grpc::ServerContext* context, const ::CompleteRequest* request,
                          ::CompleteResponse* response) override;
    grpc::Status ShowByLabel(::grpc::ServerContext* context, const ::ShowByLabelRequest* request,
                             ::ShowByLabelResponse* response) override;
    grpc::Status ShowParents(::grpc::ServerContext* context, const ::ShowParentsRequest* request,
                             ::ShowParentsResponse* response) override;
    grpc::Status ShowTask(::grpc::ServerContext* context, const ::ShowTaskRequest* request,
                          ::ShowTaskResponse* response) override;
    grpc::Status ShowAll(::grpc::ServerContext* context, const ::ShowAllRequest* request,
                         ::ShowAllResponse* response) override;
    grpc::Status Save(::grpc::ServerContext* context, const ::SaveRequest* request,
                      ::SaveResponse* response) override;
    grpc::Status Load(::grpc::ServerContext* context, const ::LoadRequest* request,
                      ::LoadResponse* response) override;

private:
    std::shared_ptr<Model> model_;
};
}