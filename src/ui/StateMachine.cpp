#include "StateMachine.h"
#include "step/Step.h"

namespace ui {

StateMachine::StateMachine(const std::shared_ptr<step::Step>& initial_step) :
        initial_step_{initial_step} {}

void StateMachine::Run(const std::shared_ptr<Controller>& controller) {
    auto result = std::make_shared<command::Result>(false);
    while (true) {
        Context context{result};
        Run(context);
        result = std::make_shared<command::Result>(context.command()->execute(controller));
        if (result->finished) break;
    }
}

void StateMachine::Run(Context& context) {
    auto step{initial_step_};
    while (!context.if_finished())
        step = step->execute(context);
}
}