#pragma once

#include <memory>
#include "ui/Context.h"
#include "ui/view/View.h"
#include "Type.h"

namespace ui {
class Factory;
namespace step {

class Step {
public:
    virtual std::shared_ptr<Step> execute(Context&) = 0;
    virtual ~Step() = default;
};

class Root : public Step {
public:
    Root(const std::shared_ptr<Factory>&, const std::shared_ptr<View>& view_);
    ~Root() override = default;

    std::shared_ptr<Step> execute(Context&) override;
private:
    const std::shared_ptr<Factory> factory_;
    const std::shared_ptr<View> view_;
};

class Quit : public Step {
public:
    Quit(const std::shared_ptr<Factory>&, const std::shared_ptr<View>&);
    ~Quit() override = default;

    std::shared_ptr<Step> execute(Context&) override;
private:
    const std::shared_ptr<Factory> factory_;
    const std::shared_ptr<View> view_;
};

class Help : public Step {
public:
    Help(const std::shared_ptr<Factory>&, const std::shared_ptr<View>&);
    ~Help() override = default;

    std::shared_ptr<Step> execute(Context&) override;
private:
    const std::shared_ptr<Factory> factory_;
    const std::shared_ptr<View> view_;
};

class Print : public Step {
public:
    Print(const std::shared_ptr<Factory>&, const std::shared_ptr<View>&);
    ~Print() override = default;

    std::shared_ptr<Step> execute(Context&) override;
private:
    const std::shared_ptr<Factory> factory_;
    const std::shared_ptr<View> view_;
};

class Complete : public Step {
public:
    Complete(const std::shared_ptr<Factory>&, const std::shared_ptr<View>&);
    ~Complete() override = default;

    std::shared_ptr<Step> execute(Context&) override;
    std::string name();
private:
    const std::shared_ptr<Factory> factory_;
    const std::shared_ptr<View> view_;
};

class Delete : public Step {
public:
    Delete(const std::shared_ptr<Factory>&, const std::shared_ptr<View>&);
    ~Delete() override = default;

    std::shared_ptr<Step> execute(Context&) override;
    std::string name();
private:
    const std::shared_ptr<Factory> factory_;
    const std::shared_ptr<View> view_;
};

class Show : public Step {
public:
    Show(const std::shared_ptr<Factory>&, const std::shared_ptr<View>&);
    ~Show() override = default;

    std::shared_ptr<Step> execute(Context&) override;
    std::string name();
private:
    const std::shared_ptr<Factory> factory_;
    const std::shared_ptr<View> view_;
};

class ShowTask : public Step {
public:
    ShowTask(const std::shared_ptr<Factory>&, const std::shared_ptr<View>&);
    ~ShowTask() override = default;

    std::shared_ptr<Step> execute(Context&) override;
    std::string name();
private:
    const std::shared_ptr<Factory> factory_;
    const std::shared_ptr<View> view_;
};

class ShowByLabel : public Step {
public:
    ShowByLabel(const std::shared_ptr<Factory>&, const std::shared_ptr<View>&);
    ~ShowByLabel() override = default;

    std::shared_ptr<Step> execute(Context&) override;
    std::string name();
private:
    const std::shared_ptr<Factory> factory_;
    const std::shared_ptr<View> view_;
};

class Save : public Step {
public:
    Save(const std::shared_ptr<Factory>&, const std::shared_ptr<View>&);
    ~Save() override = default;

    std::shared_ptr<Step> execute(Context&) override;
    std::string name();
private:
    const std::shared_ptr<Factory> factory_;
    const std::shared_ptr<View> view_;
};

class Load : public Step {
public:
    Load(const std::shared_ptr<Factory>&, const std::shared_ptr<View>&);
    ~Load() override = default;

    std::shared_ptr<Step> execute(Context&) override;
    std::string name();
private:
    const std::shared_ptr<Factory> factory_;
    const std::shared_ptr<View> view_;
};
}
}