#pragma once

#include <memory>
#include "Context.h"
#include "step/Step.h"

namespace ui {
/**
 * @brief Class that controls the executions of \b Steps
 * @details Executes commands from user until he quits the program
 * @author Michael Kupriianov
 */
class StateMachine {
public:
    /**
     * Constructor that creates an instance of the \b StateMachine
     * @param initial_step - pointer to initial instance of \b Step
     */
    explicit StateMachine(const std::shared_ptr<step::Step>& initial_step);
    virtual ~StateMachine() = default;

    /**
     * Method that manages receiving commands from the user and returning responses to him
     * @param controller - pointer to instance of \b Controller with which it is possible to communicate with the model
     * \details Methods starts another \b Run, receives a command from it, sends command to the model
     * and returns result to the \b Context
     */
    virtual void Run(const std::shared_ptr<Controller>& controller);
    /**
     * Method that manages sequential execution of \b Steps
     * @param context - instance of \b Context for steps
     */
    virtual void Run(Context& context);
private:
    const std::shared_ptr<step::Step> initial_step_;
};
}