#pragma once

#include <memory>
#include "step/Step.h"
#include "step/SubStepOwners.h"
#include "step/SubSteps.h"
#include "step/Type.h"
#include "view/View.h"

namespace ui {
/**
 * @brief Class to create \b Steps instances
 * @author Michael Kupriianov
 */
class Factory : public std::enable_shared_from_this<Factory> {
public:
    /**
     * Constructor that creates an instance of the \b Factory
     * @param view - pointer to instance of \b View class, with whom \b Steps will interact
     */
    explicit Factory(const std::shared_ptr<View>& view);
    virtual ~Factory() = default;

    /**
     * Method that creates requested \b Step
     * @param type - enum with name of necessary \b Step
     */
    virtual std::shared_ptr<step::Step> CreateStep(step::Type type);
    /**
     * Method that creates initial \b Step to start \b StateMachine execution
     */
    virtual std::shared_ptr<step::Step> GetInitialStep();

    /**
     * Method that creates initial \b SubStep to start \b StateMachine in \b SubStepOwners classes
     */
    virtual std::shared_ptr<step::Step> GetInitialSubStep();
    /**
     * Method that creates next \b SubStep from \b SubStepTitle
     */
    virtual std::shared_ptr<step::Step> GetNextSubStepFrom(const step::SubStepTitle&);
    /**
     * Method that creates next \b SubStep from \b SubStepPriority
     */
    virtual std::shared_ptr<step::Step> GetNextSubStepFrom(const step::SubStepPriority&);
    /**
     * Method that creates next \b SubStep from \b SubStepDate
     */
    virtual std::shared_ptr<step::Step> GetNextSubStepFrom(const step::SubStepDate&);
    /**
     * Method that creates next \b SubStep from \b SubStepLabel
     */
    virtual std::shared_ptr<step::Step> GetNextSubStepFrom(const step::SubStepLabel&);
private:
    const std::shared_ptr<View> view_;
};
}