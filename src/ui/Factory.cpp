#include "ui/Factory.h"

namespace ui {

Factory::Factory(const std::shared_ptr<View>& view) :
        view_{view} {}

std::shared_ptr<step::Step> Factory::CreateStep(step::Type type) {
    switch (type) {
        case step::Type::QUIT:
            return std::make_shared<step::Quit>(shared_from_this(), view_);
        case step::Type::HELP:
            return std::make_shared<step::Help>(shared_from_this(), view_);
        case step::Type::PRINT:
            return std::make_shared<step::Print>(shared_from_this(), view_);
        case step::Type::ADD:
            return std::make_shared<step::Add>(shared_from_this(), view_);
        case step::Type::ADD_SUB:
            return std::make_shared<step::AddSub>(shared_from_this(), view_);
        case step::Type::EDIT:
            return std::make_shared<step::Edit>(shared_from_this(), view_);
        case step::Type::COMPLETE:
            return std::make_shared<step::Complete>(shared_from_this(), view_);
        case step::Type::DELET:
            return std::make_shared<step::Delete>(shared_from_this(), view_);
        case step::Type::SHOW:
            return std::make_shared<step::Show>(shared_from_this(), view_);
        case step::Type::SHOW_TASK:
            return std::make_shared<step::ShowTask>(shared_from_this(), view_);
        case step::Type::SHOW_BY_LABEL:
            return std::make_shared<step::ShowByLabel>(shared_from_this(), view_);
        case step::Type::SAVE:
            return std::make_shared<step::Save>(shared_from_this(), view_);
        case step::Type::LOAD:
            return std::make_shared<step::Load>(shared_from_this(), view_);
    }
}

std::shared_ptr<step::Step> Factory::GetInitialStep() {
    return std::make_shared<step::Root>(shared_from_this(), view_);
}

std::shared_ptr<step::Step> Factory::GetInitialSubStep() {
    return std::make_shared<step::SubStepTitle>(shared_from_this(), view_);
}

std::shared_ptr<step::Step> Factory::GetNextSubStepFrom(const step::SubStepTitle&) {
    return std::make_shared<step::SubStepPriority>(shared_from_this(), view_);
}

std::shared_ptr<step::Step> Factory::GetNextSubStepFrom(const step::SubStepPriority&) {
    return std::make_shared<step::SubStepDate>(shared_from_this(), view_);
}

std::shared_ptr<step::Step> Factory::GetNextSubStepFrom(const step::SubStepDate&) {
    return std::make_shared<step::SubStepLabel>(shared_from_this(), view_);
}

std::shared_ptr<step::Step> Factory::GetNextSubStepFrom(const step::SubStepLabel&) {
    return std::make_shared<step::SubStepLabel>(shared_from_this(), view_);
}
}