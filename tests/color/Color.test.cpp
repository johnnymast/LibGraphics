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

TEST_CASE("Color white checks - brightness + min component") {
    SECTION("ExactMatchIsWhite") {
        REQUIRE(Information::is_white(255, 255, 255));
    }

    SECTION("WithinThresholdIsWhite") {
        REQUIRE(Information::is_white(230, 230, 230)); // brightness = 230, min = 230
        REQUIRE(Information::is_white(240, 240, 240)); // brightness = 240, min = 240
        REQUIRE(Information::is_white(250, 250, 250)); // brightness = 250, min = 250
    }

    SECTION("BelowThresholdIsNotWhite") {
        REQUIRE_FALSE(Information::is_white(229, 229, 229)); // brightness = 229, min = 229
        REQUIRE_FALSE(Information::is_white(200, 200, 200)); // brightness = 200
        REQUIRE_FALSE(Information::is_white(128, 128, 128)); // too dark
        REQUIRE_FALSE(Information::is_white(0, 0, 0));       // black
    }

    SECTION("OneComponentBelowThresholdIsNotWhite") {
        REQUIRE_FALSE(Information::is_white(229, 255, 255)); // min = 229 < 230
        REQUIRE_FALSE(Information::is_white(255, 229, 255)); // min = 229 < 230
        REQUIRE_FALSE(Information::is_white(255, 255, 229)); // min = 229 < 230
    }


    SECTION("CustomThresholdWorks") {
        REQUIRE(Information::is_white(250, 250, 250, 245)); // brightness = 250, min = 250
        REQUIRE(Information::is_white(245, 245, 245, 245)); // brightness = 245, min = 245
        REQUIRE_FALSE(Information::is_white(244, 244, 244, 245)); // brightness = 244
        REQUIRE_FALSE(Information::is_white(230, 230, 230, 245)); // brightness = 230
    }
}
