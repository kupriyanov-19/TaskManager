#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "ui/step/SubSteps.h"
#include "ui/FactoryMock.h"
#include "ui/view/ViewMock.h"

using ::testing::Return;

using namespace ui;
using namespace ui::step;

class SubStepTest : public ::testing::Test {
public:
    void SetUp() override {
        view_ = std::make_shared<ViewMock>(std::make_shared<Reader>(), std::make_shared<Printer>());
        factory_ = std::make_shared<FactoryMock>(view_);
    }

protected:
    std::shared_ptr<ViewMock> view_;
    std::shared_ptr<FactoryMock> factory_;
};

TEST_F(SubStepTest, shouldReadTitle) {
    SubStepTitle step{factory_, view_};
    Context context{"Add"};

    EXPECT_CALL(*view_, ReadTitle("Add")).WillOnce(Return("title"));
    EXPECT_CALL(*factory_, GetNextSubStepFrom(testing::An<const SubStepTitle&>())).Times(1);
    step.execute(context);

    EXPECT_EQ(context.task()->title(), "title");
    EXPECT_FALSE(context.if_finished());
}

TEST_F(SubStepTest, shouldReadPriority) {
    SubStepPriority step{factory_, view_};
    Context context{"Edit"};

    EXPECT_CALL(*view_, ReadPriority("Edit")).WillOnce(Return(Task_Priority_LOW));
    EXPECT_CALL(*factory_, GetNextSubStepFrom(testing::An<const SubStepPriority&>())).Times(1);
    step.execute(context);

    EXPECT_EQ(context.task()->priority(), Task_Priority_LOW);
    EXPECT_FALSE(context.if_finished());
}

TEST_F(SubStepTest, shouldReadDate) {
    SubStepDate step{factory_, view_};
    Context context{"Add"};

    google::protobuf::Timestamp date;
    date.set_seconds(100);

    EXPECT_CALL(*view_, ReadDate("Add")).WillOnce(Return(date));
    EXPECT_CALL(*factory_, GetNextSubStepFrom(testing::An<const SubStepDate&>())).Times(1);
    step.execute(context);

    ASSERT_TRUE(context.task()->has_date());
    EXPECT_EQ(context.task()->date().seconds(), 100);
    EXPECT_FALSE(context.if_finished());
}

TEST_F(SubStepTest, shouldReadLabels) {
    SubStepLabel step{factory_, view_};
    Context context{"Edit"};

    EXPECT_CALL(*view_, ReadLabels("Edit"))
            .WillOnce(Return(std::vector<std::string>{"label_1", "label_2"}));
    EXPECT_CALL(*factory_, GetNextSubStepFrom(testing::An<const SubStepLabel&>())).Times(1);
    step.execute(context);

    ASSERT_EQ(context.task()->labels().size(), 2);
    EXPECT_EQ(context.task()->labels()[0], "label_1");
    EXPECT_EQ(context.task()->labels()[1], "label_2");
    EXPECT_TRUE(context.if_finished());
}