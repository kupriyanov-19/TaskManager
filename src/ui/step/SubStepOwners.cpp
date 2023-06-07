#include "SubStepOwners.h"
#include "ui/StateMachine.h"
#include "ui/Factory.h"
#include "Task.pb.h"

namespace ui::step {

Add::Add(const std::shared_ptr<Factory>& factory, const std::shared_ptr<View>& view) :
        factory_{factory}, view_{view} {}

std::shared_ptr<Step> Add::execute(Context& context) {
    Context sub_context(name());
    StateMachine machine{factory_->GetInitialSubStep()};
    machine.Run(sub_context);

    if (view_->Confirm()) {
        context.set_command(std::make_shared<command::Add>(*sub_context.task()));
    }
    return factory_->GetInitialStep();
}

std::string Add::name() { return "[Add Task]"; }

AddSub::AddSub(const std::shared_ptr<Factory>& factory, const std::shared_ptr<View>& view) :
        factory_{factory}, view_{view} {}

std::shared_ptr<Step> AddSub::execute(Context& context) {
    TaskId parent_id{view_->ReadParentId(name())};

    Context sub_context(name());
    StateMachine machine{factory_->GetInitialSubStep()};
    machine.Run(sub_context);

    if (view_->Confirm())
        context.set_command(std::make_shared<command::AddSub>(*sub_context.task(), parent_id));
    return factory_->GetInitialStep();
}

std::string AddSub::name() { return "[Add SubTask]"; }

Edit::Edit(const std::shared_ptr<Factory>& factory, const std::shared_ptr<View>& view) :
        factory_{factory}, view_{view} {}

std::shared_ptr<Step> Edit::execute(Context& context) {
    TaskId id{view_->ReadId(name())};

    Context sub_context(name());
    StateMachine machine{factory_->GetInitialSubStep()};
    machine.Run(sub_context);

    if (view_->Confirm())
        context.set_command(std::make_shared<command::Edit>(id, *sub_context.task()));
    return factory_->GetInitialStep();
}

std::string Edit::name() { return "[Edit Task]"; }
}