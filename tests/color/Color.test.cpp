#include "LibGraphics.hpp"

#include <catch2/catch_test_macros.hpp>

using LibGraphics::Color::Information;

TEST_CASE("Color greenish checks") {
    SECTION("ExactMatchIsGreen") {
        REQUIRE(Information::is_greenish(71, 145, 114));
    }

    SECTION("WithinBufferIsGreen") {
        REQUIRE(Information::is_greenish(80, 150, 120));
    }

    SECTION("OutsideBufferIsNotGreen") {
        REQUIRE_FALSE(Information::is_greenish(10, 10, 10));
        REQUIRE_FALSE(Information::is_greenish(200, 50, 50));
    }

    SECTION("CustomBufferWorks") {
        REQUIRE(Information::is_greenish(100, 170, 140, 50));
        REQUIRE_FALSE(Information::is_greenish(100, 170, 140, 10));
    }
}