#include "ui/StateMachine.h"
#include "ui/Factory.h"
#include "ui/controller/DefaultController.h"
#include "model/Model.h"

int main(int argc, char** argv) {
    auto controller = std::make_shared<ui::DefaultController>(std::make_shared<model::Model>(
            std::make_shared<model::TaskManager>(std::make_shared<model::IdGenerator>())));
    auto factory = std::make_shared<ui::Factory>(
            std::make_shared<ui::View>(std::make_shared<ui::Reader>(), std::make_shared<ui::Printer>()));
    auto machine = std::make_shared<ui::StateMachine>(factory->GetInitialStep());

    machine->Run(controller);
    return 0;
}
