#pragma once

#include "Controller.h"
#include "model/TaskSpace.h"

namespace ui {
class DefaultController : public Controller {
public:
    explicit DefaultController(const std::shared_ptr<model::TaskSpace>& model) : model_{model} {}
    ~DefaultController() override = default;

    bool AddTask(const Task& task) override { return model_->AddTask(task); }
    bool AddSubTask(const Task& task, const TaskId& id) override { return model_->AddSubTask(task, id); }
    bool Edit(const TaskId& id, const Task& task) override { return model_->Edit(id, task); }
    bool Complete(const TaskId& id) override { return model_->Complete(id); }
    bool Delete(const TaskId& id) override { return model_->Delete(id); }

    ManyTasksWithId ShowByLabel(const std::string& label, const TasksSortBy& sort_by) override {
        return model_->ShowByLabel(label, sort_by);
    }
    ManyTasksWithId ShowParents(const TasksSortBy& sort_by) override {
        return model_->ShowParents(sort_by);
    }
    CompositeTask ShowTask(const TaskId& id, const TasksSortBy& sort_by) override {
        return model_->ShowTask(id, sort_by);
    }
    ManyCompositeTasks ShowAll(const TasksSortBy& sort_by) override {
        return model_->ShowAll(sort_by);
    }

    bool Save(const std::string& filename) override { return model_->Save(filename); }
    bool Load(const std::string& filename) override { return model_->Load(filename); }

    bool Create(const std::string& filename, const std::string& password) override { return model_->CreateSpace(filename, password); }
    bool Enter(const std::string& filename, const std::string& password) override { return model_->EnterSpace(filename, password); }
    bool DeleteSpace(const std::string& filename, const std::string& password) override {return model_->DeleteSpace(filename, password);}
private:
    const std::shared_ptr<model::TaskSpace> model_;
};
}
