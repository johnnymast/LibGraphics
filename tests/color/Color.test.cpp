#include "LibGraphics.hpp"

#include <catch2/catch_test_macros.hpp>

using LibGraphics::Color::Information;

TEST_CASE("Color greenish checks", "[Information][is_greenish]") {
    SECTION("ExactMatchIsGreen") {
        // Exact target kleur #708d00 (112,141,0)
        REQUIRE(Information::is_greenish(112, 141, 0, 25));
    }

    SECTION("WithinBufferIsGreen") {
        // Binnen ±25 van target
        REQUIRE(Information::is_greenish(120, 150, 10, 25));
        REQUIRE(Information::is_greenish(100, 130, 5, 25));
    }

    SECTION("OutsideBufferIsNotGreen") {
        REQUIRE_FALSE(Information::is_greenish(10, 10, 10, 25));
        REQUIRE_FALSE(Information::is_greenish(200, 200, 200, 25));
    }

    SECTION("CustomBufferWorks") {
        // Met buffer 50 valt deze kleur binnen bereik
        REQUIRE(Information::is_greenish(100, 170, 40, 50));
        // Met buffer 10 valt dezelfde kleur buiten bereik
        REQUIRE_FALSE(Information::is_greenish(100, 170, 40, 10));
    }
}

TEST_CASE("Color white checks - brightness + min component + chroma") {
    SECTION("ExactMatchIsWhite") {
        REQUIRE(Information::is_white(255, 255, 255)); // puur wit
    }

    SECTION("WithinThresholdIsWhite") {
        REQUIRE(Information::is_white(230, 230, 230)); // lichtgrijs
        REQUIRE(Information::is_white(240, 240, 240)); // lichtgrijs
        REQUIRE(Information::is_white(250, 250, 250)); // bijna puur wit
        REQUIRE(Information::is_white(222, 222, 222)); // jouw voorbeeld witte tekst
    }

    SECTION("BelowThresholdIsNotWhite") {
        REQUIRE_FALSE(Information::is_white(200, 200, 200)); // donkergrijs
        REQUIRE_FALSE(Information::is_white(128, 128, 128)); // donker
        REQUIRE_FALSE(Information::is_white(0, 0, 0));       // zwart
        REQUIRE_FALSE(Information::is_white(128, 148, 155)); // Davie (offline)
    }

    SECTION("OneComponentBelowThresholdIsNotWhite") {
        REQUIRE_FALSE(Information::is_white(219, 255, 255)); // min=219 < 220
        REQUIRE_FALSE(Information::is_white(255, 219, 255)); // min=219 < 220
        REQUIRE_FALSE(Information::is_white(255, 255, 219)); // min=219 < 220
    }

    SECTION("CustomThresholdWorks") {
        REQUIRE(Information::is_white(250, 250, 250, 245)); // brightness=250, min=250
        REQUIRE(Information::is_white(245, 245, 245, 245)); // brightness=245, min=245
        REQUIRE_FALSE(Information::is_white(244, 244, 244, 245)); // brightness=244 < 245
        REQUIRE_FALSE(Information::is_white(230, 230, 230, 245)); // brightness=230 < 245
    }

    SECTION("ChromaDifferenceBlocksNonWhiteColors") {
        REQUIRE_FALSE(Information::is_white(255, 200, 200)); // te roodachtig
        REQUIRE_FALSE(Information::is_white(200, 255, 200)); // te groenachtig
        REQUIRE_FALSE(Information::is_white(200, 200, 255)); // te blauwachtig
        REQUIRE_FALSE(Information::is_white(230, 220, 240)); // lichte kleurzweem, diff > 10
    }
}



