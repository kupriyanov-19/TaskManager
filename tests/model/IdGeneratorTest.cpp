#include "gtest/gtest.h"
#include "model/IdGenerator.h"

class IdGeneratorTest : public ::testing::Test {
};

TEST_F(IdGeneratorTest, shouldGenerateID) {
    model::IdGenerator generator;
    EXPECT_EQ(generator.GenerateId().value(), 0);
    EXPECT_EQ(generator.GenerateId().value(), 1);
    EXPECT_EQ(generator.GenerateId().value(), 2);
}

TEST_F(IdGeneratorTest, shouldCreateGenerator) {
    model::IdGenerator generator(15);
    EXPECT_EQ(generator.GenerateId().value(), 15);
    EXPECT_EQ(generator.GenerateId().value(), 16);
    EXPECT_EQ(generator.GenerateId().value(), 17);
}

