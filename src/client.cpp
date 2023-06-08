#include <string>
#include <grpcpp/grpcpp.h>
#include "ModelService.grpc.pb.h"
#include "ui/controller/GRPCEndPoint.h"
#include "ui/StateMachine.h"
#include "ui/Factory.h"

int main(int argc, char **argv) {
    std::cout << "Where do you want to connect? Enter in the format 'host:port'";
    std::string target_str;
    std::cin >> target_str;
    if (target_str=="") target_str = "localhost:1234";

    auto factory = std::make_shared<ui::Factory>(
            std::make_shared<ui::View>(std::make_shared<ui::Reader>(), std::make_shared<ui::Printer>()));

    auto machine = std::make_shared<ui::StateMachine>(factory->GetInitialStep());

    std::shared_ptr<ui::Controller> controller =
            std::make_shared<ui::GRPCEndPoint>(
                    ModelService::NewStub(grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials())));

    machine->Run(controller);
}