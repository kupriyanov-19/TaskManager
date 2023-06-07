#include "ui/StateMachine.h"
#include "ui/Factory.h"
#include "ui/controller/DefaultController.h"
#include "model/Model.h"
//#include "logging/Initialisation.h"
//#include <boost/program_options.hpp>

//namespace options = boost::program_options;

int main(int argc, char** argv) {
//    options::options_description general_options("Available options");
//    general_options.add_options()
//            ("debug", "debug logging mode");

//    options::variables_map arguments;
//    options::store(options::parse_command_line(argc, argv, general_options), arguments);
//    options::notify(arguments);

//    ConsoleLogging{boost::log::trivial::error};

//    (arguments.find("debug") != arguments.end()) ? FileLogging{"main.log", boost::log::trivial::debug} :
//                                                   FileLogging{"main.log", boost::log::trivial::info};

    auto controller = std::make_shared<ui::DefaultController>(std::make_shared<model::Model>(
            std::make_shared<model::TaskManager>(std::make_shared<model::IdGenerator>())));
    auto factory = std::make_shared<ui::Factory>(
            std::make_shared<ui::View>(std::make_shared<ui::Reader>(), std::make_shared<ui::Printer>()));
    auto machine = std::make_shared<ui::StateMachine>(factory->GetInitialStep());

    machine->Run(controller);
    return 0;
}
