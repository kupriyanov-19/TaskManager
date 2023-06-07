#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include "model/GRPCEndPoint.h"
#include "model/Model.h"
//#include "logging/Initialisation.h"
//#include "logging/Log.h"
//#include <boost/program_options.hpp>

//namespace options = boost::program_options;

int main(int argc, char** argv) {
//    std::string port;
//    options::options_description general_options("Available options");
//    general_options.add_options()
//            ("debug", "debug logging mode")
//            ("port,p", options::value<std::string>(&port)->default_value("1234"));

//    options::variables_map arguments;
//    options::store(options::parse_command_line(argc, argv, general_options), arguments);
//    options::notify(arguments);

//    ConsoleLogging{boost::log::trivial::error};

//    (arguments.find("debug") != arguments.end()) ? FileLogging{"server.log", boost::log::trivial::debug} :
//                                                   FileLogging{"server.log", boost::log::trivial::info};

    auto model = std::make_shared<model::Model>(
            std::make_shared<model::TaskManager>(std::make_shared<model::IdGenerator>()));
    model::GRPCEndPoint service{model};
//    std::string server_address = "0.0.0.0:" + port;
    std::string server_address = "0.0.0.0:1234";

    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
//    LOG(info, "Server listening on " + server_address);

    server->Wait();
}