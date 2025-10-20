#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "LibGraphics.hpp"

using namespace LibGraphics::Match;

TEST_CASE("DResult default constructor", "[DResult]") {
    MatchResult result;

    REQUIRE(result.X == 0);
    REQUIRE(result.Y == 0);
    REQUIRE(result.Width == 0);
    REQUIRE(result.Height == 0);
    REQUIRE(result.Score == 0.0f);
}

TEST_CASE("DResult parameterized constructor", "[DResult]") {
    SECTION("All parameters provided") {
        MatchResult result(10, 20, 100, 50, 0.95f);

        REQUIRE(result.X == 10);
        REQUIRE(result.Y == 20);
        REQUIRE(result.Width == 100);
        REQUIRE(result.Height == 50);
        REQUIRE_THAT(result.Score, Catch::Matchers::WithinRel(0.95f, 0.0001f));
    }

    SECTION("Partial parameters provided") {
        MatchResult result(5, 15);

        REQUIRE(result.X == 5);
        REQUIRE(result.Y == 15);
        REQUIRE(result.Width == 0);
        REQUIRE(result.Height == 0);
        REQUIRE(result.Score == 0.0f);
    }

    SECTION("With score only") {
        MatchResult result(0, 0, 0, 0, 0.75f);

        REQUIRE(result.X == 0);
        REQUIRE(result.Y == 0);
        REQUIRE(result.Width == 0);
        REQUIRE(result.Height == 0);
        REQUIRE_THAT(result.Score, Catch::Matchers::WithinRel(0.75f, 0.0001f));
    }
}

TEST_CASE("DResult Center calculation", "[DResult][Center]") {
    SECTION("Center with even dimensions") {
        MatchResult result(0, 0, 100, 50, 0.9f);
        MatchResult center = result.Center();

        REQUIRE(center.X == 50);   // 0 + 100/2
        REQUIRE(center.Y == 25);   // 0 + 50/2
        REQUIRE(center.Width == 100);
        REQUIRE(center.Height == 50);
        REQUIRE_THAT(center.Score, Catch::Matchers::WithinRel(0.0f, 0.0001f));
    }

    SECTION("Center with odd dimensions") {
        MatchResult result(0, 0, 101, 51, 0.85f);
        MatchResult center = result.Center();

        REQUIRE(center.X == 50);   // 0 + 101/2 = 50 (integer division)
        REQUIRE(center.Y == 25);   // 0 + 51/2 = 25 (integer division)
        REQUIRE(center.Width == 101);
        REQUIRE(center.Height == 51);
    }

    SECTION("Center with non-zero origin") {
        MatchResult result(100, 200, 60, 40, 0.99f);
        MatchResult center = result.Center();

        REQUIRE(center.X == 130);  // 100 + 60/2
        REQUIRE(center.Y == 220);  // 200 + 40/2
        REQUIRE(center.Width == 60);
        REQUIRE(center.Height == 40);
    }

    SECTION("Center with negative coordinates") {
        MatchResult result(-10, -20, 40, 60, 0.5f);
        MatchResult center = result.Center();

        REQUIRE(center.X == 10);   // -10 + 40/2
        REQUIRE(center.Y == 10);   // -20 + 60/2
        REQUIRE(center.Width == 40);
        REQUIRE(center.Height == 60);
    }

    SECTION("Center with zero dimensions") {
        MatchResult result(50, 100, 0, 0, 1.0f);
        MatchResult center = result.Center();

        REQUIRE(center.X == 50);   // 50 + 0/2
        REQUIRE(center.Y == 100);  // 100 + 0/2
        REQUIRE(center.Width == 0);
        REQUIRE(center.Height == 0);
    }

    SECTION("Center called multiple times") {
        MatchResult result(0, 0, 100, 100, 0.8f);
        MatchResult center1 = result.Center();
        MatchResult center2 = center1.Center();

        REQUIRE(center1.X == 50);
        REQUIRE(center1.Y == 50);

        REQUIRE(center2.X == 100);  // 50 + 100/2
        REQUIRE(center2.Y == 100);  // 50 + 100/2
    }

    SECTION("Center with large values") {
        MatchResult result(1000, 2000, 5000, 3000, 0.95f);
        MatchResult center = result.Center();

        REQUIRE(center.X == 3500);   // 1000 + 5000/2
        REQUIRE(center.Y == 3500);   // 2000 + 3000/2
        REQUIRE(center.Width == 5000);
        REQUIRE(center.Height == 3000);
    }
}

TEST_CASE("DResult score handling", "[DResult][Score]") {
    SECTION("Score range 0.0 to 1.0") {
        MatchResult result1(0, 0, 10, 10, 0.0f);
        MatchResult result2(0, 0, 10, 10, 0.5f);
        MatchResult result3(0, 0, 10, 10, 1.0f);

        REQUIRE(result1.Score == 0.0f);
        REQUIRE_THAT(result2.Score, Catch::Matchers::WithinRel(0.5f, 0.0001f));
        REQUIRE_THAT(result3.Score, Catch::Matchers::WithinRel(1.0f, 0.0001f));
    }

    SECTION("Score precision") {
        MatchResult result(10, 20, 30, 40, 0.123456f);
        REQUIRE_THAT(result.Score, Catch::Matchers::WithinRel(0.123456f, 0.0001f));
    }

    SECTION("Negative scores") {
        MatchResult result(0, 0, 10, 10, -0.5f);
        REQUIRE_THAT(result.Score, Catch::Matchers::WithinRel(-0.5f, 0.0001f));
    }

    SECTION("Scores greater than 1.0") {
        MatchResult result(0, 0, 10, 10, 1.5f);
        REQUIRE_THAT(result.Score, Catch::Matchers::WithinRel(1.5f, 0.0001f));
    }
}

TEST_CASE("DResult copy behavior", "[DResult]") {
    MatchResult result1(10, 20, 30, 40, 0.88f);
    MatchResult result2 = result1;

    REQUIRE(result2.X == 10);
    REQUIRE(result2.Y == 20);
    REQUIRE(result2.Width == 30);
    REQUIRE(result2.Height == 40);
    REQUIRE_THAT(result2.Score, Catch::Matchers::WithinRel(0.88f, 0.0001f));

    SECTION("Copies are independent") {
        result2.X = 100;
        result2.Score = 0.5f;

        REQUIRE(result1.X == 10);
        REQUIRE(result2.X == 100);
        REQUIRE_THAT(result1.Score, Catch::Matchers::WithinRel(0.88f, 0.0001f));
        REQUIRE_THAT(result2.Score, Catch::Matchers::WithinRel(0.5f, 0.0001f));
    }
}

TEST_CASE("DResult const correctness", "[DResult][Center]") {
    SECTION("Center is const-qualified") {
        const MatchResult result(10, 20, 30, 40, 0.95f);
        MatchResult center = result.Center();  // Should compile because Center() is const

        REQUIRE(center.X == 25);
        REQUIRE(center.Y == 40);
    }
}

TEST_CASE("DResult typical use cases", "[DResult][Integration]") {
    SECTION("Template matching result") {
        // Simulating a template match result from OpenCV
        MatchResult match(150, 200, 64, 64, 0.987f);

        REQUIRE(match.X == 150);
        REQUIRE(match.Y == 200);
        REQUIRE(match.Width == 64);
        REQUIRE(match.Height == 64);
        REQUIRE_THAT(match.Score, Catch::Matchers::WithinRel(0.987f, 0.0001f));

        // Get center point for drawing or further processing
        MatchResult center = match.Center();
        REQUIRE(center.X == 182);  // 150 + 64/2
        REQUIRE(center.Y == 232);  // 200 + 64/2
    }

    SECTION("Multiple results comparison") {
        MatchResult best(100, 100, 50, 50, 0.99f);
        MatchResult good(200, 200, 50, 50, 0.85f);
        MatchResult poor(300, 300, 50, 50, 0.60f);

        REQUIRE(best.Score > good.Score);
        REQUIRE(good.Score > poor.Score);
    }

    SECTION("Result filtering by score threshold") {
        MatchResult result1(0, 0, 10, 10, 0.95f);
        MatchResult result2(10, 10, 10, 10, 0.75f);
        MatchResult result3(20, 20, 10, 10, 0.45f);

        const float threshold = 0.8f;

        REQUIRE(result1.Score >= threshold);
        REQUIRE(result2.Score < threshold);
        REQUIRE(result3.Score < threshold);
    }
}

TEST_CASE("DResult edge cases", "[DResult][Edge]") {
    SECTION("Maximum integer values") {
        MatchResult result(INT32_MAX, INT32_MAX, INT32_MAX, INT32_MAX, 1.0f);

        REQUIRE(result.X == INT32_MAX);
        REQUIRE(result.Y == INT32_MAX);
        REQUIRE(result.Width == INT32_MAX);
        REQUIRE(result.Height == INT32_MAX);
    }

    SECTION("Minimum integer values") {
        MatchResult result(INT32_MIN, INT32_MIN, 0, 0, 0.0f);

        REQUIRE(result.X == INT32_MIN);
        REQUIRE(result.Y == INT32_MIN);
    }

    SECTION("Very small score values") {
        MatchResult result(0, 0, 10, 10, 0.000001f);
        REQUIRE_THAT(result.Score, Catch::Matchers::WithinRel(0.000001f, 0.000001f));
    }
}