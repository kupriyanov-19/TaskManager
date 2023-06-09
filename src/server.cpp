#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include "model/GRPCEndPoint.h"
#include "model/TaskSpace.h"

int main(int argc, char** argv) {
    auto model = std::make_shared<model::TaskSpace>(
            std::make_shared<model::TaskManager>(std::make_shared<model::IdGenerator>()));
    model::GRPCEndPoint service{model};

    std::cout<<"Where do you want to listen? Enter in the format: 'port'";
    std::string server_address;
    std::cin>>server_address;
    if (server_address=="def") server_address = "0.0.0.0:1234";
    else server_address = "0.0.0.0:"+ server_address;

    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());

    server->Wait();
}