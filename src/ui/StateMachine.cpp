#include "StateMachine.h"
#include "step/Step.h"

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
            const std::string name{view->ReadName("[Create space of tasks]")};
            const std::string password1{view->ReadPassword("[Create space of tasks]")};
            const std::string password2{view->ReadPassword("[Create space of tasks] repeat")};
            if (password1!=password2) view->PrintString("Passwords are not equivalent");
            else {
                if (view->Confirm()) {
                    bool g = controller->Create(name, password1);
                    if (g) {
                        view->PrintString("Space of tasks successfully created");
                        nm=name;
                        break;
                    }
                    else view->PrintString("This space of tasks already exists");
                }
            }
        }
        if (c==step::Type::ENTER) {
            const std::string name{view->ReadName("[Enter space of tasks]")};
            const std::string password{view->ReadPassword("[Enter space of tasks]")};
            bool g = controller->Enter(name, password);
            if (g) {
                view->PrintString("You successfully entered the space of tasks: " + name);
                nm=name;
                break;
            }
            else view->PrintString("Wrong name or password");
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
}