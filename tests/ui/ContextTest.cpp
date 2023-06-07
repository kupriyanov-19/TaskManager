#include "gtest/gtest.h"
#include "ui/Context.h"

using namespace ui;

class ContextTest : public ::testing::Test {
};

TEST_F(ContextTest, shouldWorkWithCommand) {
    Context context{std::make_shared<command::Result>(false)};
    auto command = std::make_shared<command::Quit>();
    context.set_command(command);

    EXPECT_TRUE(std::dynamic_pointer_cast<command::Quit>(std::shared_ptr(
            context.command())));
}

TEST_F(ContextTest, shouldWorkWithResult) {
    ui::Context context{std::make_shared<command::Result>(true)};
    EXPECT_TRUE(context.result()->finished);
    context.set_result();
    EXPECT_FALSE(context.result()->finished);
}

TEST_F(ContextTest, shouldWorkWithTask) {
    Context context{""};
    context.task()->set_title("title");
    context.task()->set_priority(Task_Priority_LOW);
    context.task()->add_labels("qwerty");

    EXPECT_EQ(context.task()->title(), "title");
    EXPECT_EQ(context.task()->priority(), Task_Priority_LOW);
    EXPECT_FALSE(context.task()->has_date());
    EXPECT_EQ(context.task()->labels().size(), 1);
    EXPECT_EQ(context.task()->labels()[0], "qwerty");
}

TEST_F(ContextTest, shouldWorkWithWizardString) {
    Context context{"Add"};
    EXPECT_EQ(context.wizard_string(), "Add");
}

TEST_F(ContextTest, shouldFinished) {
    Context context{""};
    EXPECT_EQ(context.if_finished(), false);
    context.finished();
    EXPECT_EQ(context.if_finished(), true);
}