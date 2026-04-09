#include <catch2/catch_test_macros.hpp>
#include "LibGraphics/type/Rect.hpp"

using LibGraphics::Type::Rect;

TEST_CASE("Rect contains point", "[Rect]") {
    Rect r{10, 20, 30, 40};
    REQUIRE(r.contains(15, 25));   // binnen
    REQUIRE_FALSE(r.contains(5, 25));  // buiten links
    REQUIRE_FALSE(r.contains(15, 70)); // buiten onder
}

TEST_CASE("Rect area calculation", "[Rect]") {
    Rect r{0, 0, 10, 5};
    REQUIRE(r.area() == 50);
}

TEST_CASE("Rect intersection", "[Rect]") {
    Rect a{0, 0, 10, 10};
    Rect b{5, 5, 10, 10};
    Rect inter = a.intersect(b);

    REQUIRE(inter.X == 5);
    REQUIRE(inter.Y == 5);
    REQUIRE(inter.Width == 5);
    REQUIRE(inter.Height == 5);
}

TEST_CASE("Rect intersection empty", "[Rect]") {
    Rect a{0, 0, 10, 10};
    Rect b{20, 20, 5, 5};
    Rect inter = a.intersect(b);

    REQUIRE(inter.Width == 0);
    REQUIRE(inter.Height == 0);
}
