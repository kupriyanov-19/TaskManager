#include <string>
#include <grpcpp/grpcpp.h>
#include "ModelService.grpc.pb.h"
#include "ui/controller/GRPCEndPoint.h"
#include "ui/StateMachine.h"
#include "ui/Factory.h"

int main(int argc, char **argv) {
    while (true) {
        auto view = std::make_shared<ui::View>(std::make_shared<ui::Reader>(), std::make_shared<ui::Printer>());
        auto factory = std::make_shared<ui::Factory>(view);
        auto machine = std::make_shared<ui::StateMachine>(factory->GetInitialStep());

        std::string target_str{view->ReadIP()};

        std::shared_ptr<ui::Controller> controller =
                std::make_shared<ui::GRPCEndPoint>(
                        ModelService::NewStub(grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials())));

        if (machine->Check(controller)) {
            machine->Run(controller);
            view->PrintCheckResult(true);
            break;
        }
        else view->PrintCheckResult(false);
    }
}