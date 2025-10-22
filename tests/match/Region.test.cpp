#include "LibGraphics/match/Region.hpp"

#include <catch2/catch_test_macros.hpp>

using namespace LibGraphics::Match;

TEST_CASE("Region default constructor", "[Region]") {
    Region r;

    REQUIRE(r.X == 0);
    REQUIRE(r.Y == 0);
    REQUIRE(r.Width == 0);
    REQUIRE(r.Height == 0);
}

TEST_CASE("Region parameterized constructor", "[Region]") {
    SECTION("All parameters provided") {
        Region r(10, 20, 100, 50);

        REQUIRE(r.X == 10);
        REQUIRE(r.Y == 20);
        REQUIRE(r.Width == 100);
        REQUIRE(r.Height == 50);
    }

    SECTION("Partial parameters provided") {
        Region r(5, 15);

        REQUIRE(r.X == 5);
        REQUIRE(r.Y == 15);
        REQUIRE(r.Width == 0);
        REQUIRE(r.Height == 0);
    }
}

TEST_CASE("Region Center calculation", "[Region][Center]") {
    SECTION("Center with even dimensions") {
        Region r(0, 0, 100, 50);
        Region center = r.Center();

        REQUIRE(center.X == 50);   // 0 + 100/2
        REQUIRE(center.Y == 25);   // 0 + 50/2
        REQUIRE(center.Width == 100);
        REQUIRE(center.Height == 50);
    }

    SECTION("Center with odd dimensions") {
        Region r(0, 0, 101, 51);
        Region center = r.Center();

        REQUIRE(center.X == 50);   // 0 + 101/2 = 50 (integer division)
        REQUIRE(center.Y == 25);   // 0 + 51/2 = 25 (integer division)
        REQUIRE(center.Width == 101);
        REQUIRE(center.Height == 51);
    }

    SECTION("Center with non-zero origin") {
        Region r(100, 200, 60, 40);
        Region center = r.Center();

        REQUIRE(center.X == 130);  // 100 + 60/2
        REQUIRE(center.Y == 220);  // 200 + 40/2
        REQUIRE(center.Width == 60);
        REQUIRE(center.Height == 40);
    }

    SECTION("Center with negative coordinates") {
        Region r(-10, -20, 40, 60);
        Region center = r.Center();

        REQUIRE(center.X == 10);   // -10 + 40/2
        REQUIRE(center.Y == 10);   // -20 + 60/2
        REQUIRE(center.Width == 40);
        REQUIRE(center.Height == 60);
    }

    SECTION("Center with zero dimensions") {
        Region r(50, 100, 0, 0);
        Region center = r.Center();

        REQUIRE(center.X == 50);   // 50 + 0/2
        REQUIRE(center.Y == 100);  // 100 + 0/2
        REQUIRE(center.Width == 0);
        REQUIRE(center.Height == 0);
    }

    SECTION("Center called multiple times") {
        Region r(0, 0, 100, 100);
        Region center1 = r.Center();
        Region center2 = center1.Center();

        REQUIRE(center1.X == 50);
        REQUIRE(center1.Y == 50);

        REQUIRE(center2.X == 100);  // 50 + 100/2
        REQUIRE(center2.Y == 100);  // 50 + 100/2
    }

    SECTION("Center with large values") {
        Region r(1000, 2000, 5000, 3000);
        Region center = r.Center();

        REQUIRE(center.X == 3500);   // 1000 + 5000/2
        REQUIRE(center.Y == 3500);   // 2000 + 3000/2
        REQUIRE(center.Width == 5000);
        REQUIRE(center.Height == 3000);
    }
}

TEST_CASE("Region copy behavior", "[Region]") {
    Region r1(10, 20, 30, 40);
    Region r2 = r1;

    REQUIRE(r2.X == 10);
    REQUIRE(r2.Y == 20);
    REQUIRE(r2.Width == 30);
    REQUIRE(r2.Height == 40);

    SECTION("Copies are independent") {
        r2.X = 100;
        REQUIRE(r1.X == 10);
        REQUIRE(r2.X == 100);
    }
}

TEST_CASE("Region const correctness", "[Region][Center]") {
    SECTION("Center is const-qualified") {
        const Region r(10, 20, 30, 40);
        Region center = r.Center();  // Should compile because Center() is const

        REQUIRE(center.X == 25);
        REQUIRE(center.Y == 40);
    }
}