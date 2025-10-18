#include <gtest/gtest.h>
#include "helpers/color.hpp"

using helpers::Color;

TEST(ColorTest, ExactMatchIsGreen) {
    EXPECT_TRUE(Color::is_greenish(71, 145, 114));
}

TEST(ColorTest, WithinBufferIsGreen) {
    EXPECT_TRUE(Color::is_greenish(80, 150, 120));
}

TEST(ColorTest, OutsideBufferIsNotGreen) {
    EXPECT_FALSE(Color::is_greenish(10, 10, 10));
    EXPECT_FALSE(Color::is_greenish(200, 50, 50));
}

TEST(ColorTest, CustomBufferWorks) {
    EXPECT_TRUE(Color::is_greenish(100, 170, 140, 50));
    EXPECT_FALSE(Color::is_greenish(100, 170, 140, 10));
}
