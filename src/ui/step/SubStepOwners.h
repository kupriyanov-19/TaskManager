#pragma once

#include <memory>
#include "Step.h"
#include "ui/Context.h"

namespace ui::step {

class Add : public Step {
public:
    Add(const std::shared_ptr<Factory>&, const std::shared_ptr<View>&);
    ~Add() override = default;

    std::shared_ptr<Step> execute(Context&) override;
    std::string name();
private:
    const std::shared_ptr<Factory> factory_;
    const std::shared_ptr<View> view_;
};

class AddSub : public Step {
public:
    AddSub(const std::shared_ptr<Factory>&, const std::shared_ptr<View>&);
    ~AddSub() override = default;

    std::shared_ptr<Step> execute(Context&) override;
    std::string name();
private:
    const std::shared_ptr<Factory> factory_;
    const std::shared_ptr<View> view_;
};

class Edit : public Step {
public:
    Edit(const std::shared_ptr<Factory>&, const std::shared_ptr<View>&);
    ~Edit() override = default;

    std::shared_ptr<Step> execute(Context&) override;
    std::string name();
private:
    const std::shared_ptr<Factory> factory_;
    const std::shared_ptr<View> view_;
};
}

