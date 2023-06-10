#include <string>
#include <grpcpp/grpcpp.h>
#include "ModelService.grpc.pb.h"
#include "ui/controller/GRPCEndPoint.h"
#include "ui/StateMachine.h"
#include "ui/Factory.h"
#include "utilities/CreateProtoObjects.h"

int main(int argc, char **argv) {
    while (true) {
        std::cout << "Where do you want to connect? Enter in the format 'host:port'";
        std::string target_str;
        std::cin >> target_str;
        std::cin.ignore(3000, '\n');
        if (target_str == "default") target_str = "localhost:1234";
        if (target_str == "def") target_str = "192.168.0.104:1234";

        auto factory = std::make_shared<ui::Factory>(
                std::make_shared<ui::View>(std::make_shared<ui::Reader>(), std::make_shared<ui::Printer>()));

        auto machine = std::make_shared<ui::StateMachine>(factory->GetInitialStep());

        std::shared_ptr<ui::Controller> controller =
                std::make_shared<ui::GRPCEndPoint>(
                        ModelService::NewStub(grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials())));

        Task t(CreateTask(""));
        controller->AddTask(t);
        auto s = controller->ShowAll(TasksSortBy::ID);
        if (s.tasks().size() >= 1) {
            std::cout << "You successfully connected to the server\n";
            machine->Run(controller);
            break;
        }
        else std::cout<< "This server is unavailable, please try again\n";
    }
}