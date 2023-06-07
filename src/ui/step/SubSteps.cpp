#include "SubSteps.h"
#include "ui/Factory.h"
#include "Task.pb.h"

namespace ui::step {

SubStepTitle::SubStepTitle(const std::shared_ptr<Factory>& factory, const std::shared_ptr<View>& view) :
        factory_{factory}, view_{view} {}

std::shared_ptr<Step> SubStepTitle::execute(Context& context) {
    const std::string title{view_->ReadTitle(context.wizard_string())};
    context.task()->set_title(title);
    return factory_->GetNextSubStepFrom(*this);
}

SubStepPriority::SubStepPriority(const std::shared_ptr<Factory>& factory, const std::shared_ptr<View>& view) :
        factory_{factory}, view_{view} {}

std::shared_ptr<Step> SubStepPriority::execute(Context& context) {
    Task::Priority priority(view_->ReadPriority(context.wizard_string()));
    context.task()->set_priority(priority);
    return factory_->GetNextSubStepFrom(*this);
}

SubStepDate::SubStepDate(const std::shared_ptr<Factory>& factory, const std::shared_ptr<View>& view) :
        factory_{factory}, view_{view} {}

std::shared_ptr<Step> SubStepDate::execute(Context& context) {
    google::protobuf::Timestamp date(view_->ReadDate(context.wizard_string()));
    context.task()->set_allocated_date(std::make_unique<google::protobuf::Timestamp>(date).release());
    return factory_->GetNextSubStepFrom(*this);
}

SubStepLabel::SubStepLabel(const std::shared_ptr<Factory>& factory, const std::shared_ptr<View>& view) :
        factory_{factory}, view_{view} {}

std::shared_ptr<Step> SubStepLabel::execute(Context& context) {
    const std::vector<std::string> labels{view_->ReadLabels(context.wizard_string())};
    for (const auto& label: labels)
        context.task()->add_labels(label);
    context.finished();
    return factory_->GetNextSubStepFrom(*this);
}
}