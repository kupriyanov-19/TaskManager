#include "StateMachine.h"
#include "step/Step.h"
#include "utilities/CreateProtoObjects.h"

namespace ui {

StateMachine::StateMachine(const std::shared_ptr<step::Step>& initial_step) :
        initial_step_{initial_step} {}

void StateMachine::Run(const std::shared_ptr<Controller>& controller) {
    auto view(std::make_shared<ui::View>(std::make_shared<ui::Reader>(), std::make_shared<ui::Printer>()));
    std::string nm;
    while (true) {
        auto c = view->ReadFirstCommand();
        if (c==step::Type::HELP) view->PrintGlobalHelp();
        if (c==step::Type::MAIN_QUIT) {view->PrintQuit(); return;}
        if (c==step::Type::CREATE) {
            std::string name, password;
            if (view->ReadCreateData(name, password)) {
                bool g = controller->Create(name, password);
                view->PrintCreateResult(g);
                if (g) break;
            }
        }
        if (c==step::Type::ENTER) {
            std::string name, password;
            view->ReadEnterData(name, password);
            bool g = controller->Enter(name, password);
            view->PrintEnterResult(g, name);
            if (g) break;
        }
        if (c==step::Type::DELETE_SPACE) {
            std::string name, password;
            view->ReadDeleteData(name, password);
            bool g = controller->DeleteSpace(name, password);
            view->PrintDeleteResult(g, name);
        }
    }

    auto result = std::make_shared<command::Result>(false);
    while (true) {
        Context context{result};
        Run(context);
        result = std::make_shared<command::Result>(context.command()->execute(controller));
        if (result->finished) {controller->Save(nm); Run(controller); break;}
    }
}

void StateMachine::Run(Context& context) {
    auto step{initial_step_};
    while (!context.if_finished())
        step = step->execute(context);
}

bool StateMachine::Check(const std::shared_ptr<Controller>& controller) {
    Task t(CreateTask(""));
    controller->AddTask(t);
    auto s = controller->ShowAll(TasksSortBy::ID);
    if (s.tasks().size() >= 1) return true;
    return false;
}

}