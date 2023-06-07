#pragma once

#include <memory>
#include "ui/Context.h"
#include "Step.h"

namespace ui::step {

class SubStepTitle : public Step {
public:
    SubStepTitle(const std::shared_ptr<Factory>&, const std::shared_ptr<View>&);
    ~SubStepTitle() override = default;

    std::shared_ptr<Step> execute(Context&) override;
private:
    const std::shared_ptr<Factory> factory_;
    const std::shared_ptr<View> view_;
};

class SubStepPriority : public Step {
public:
    SubStepPriority(const std::shared_ptr<Factory>&, const std::shared_ptr<View>&);
    ~SubStepPriority() override = default;

    std::shared_ptr<Step> execute(Context&) override;
private:
    const std::shared_ptr<Factory> factory_;
    const std::shared_ptr<View> view_;
};

class SubStepDate : public Step {
public:
    SubStepDate(const std::shared_ptr<Factory>&, const std::shared_ptr<View>&);
    ~SubStepDate() override = default;

    std::shared_ptr<Step> execute(Context&) override;
private:
    const std::shared_ptr<Factory> factory_;
    const std::shared_ptr<View> view_;
};

class SubStepLabel : public Step {
public:
    SubStepLabel(const std::shared_ptr<Factory>&, const std::shared_ptr<View>&);
    ~SubStepLabel() override = default;

    std::shared_ptr<Step> execute(Context&) override;
private:
    const std::shared_ptr<Factory> factory_;
    const std::shared_ptr<View> view_;
};
}