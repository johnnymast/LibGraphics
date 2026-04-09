// #define STB_IMAGE_IMPLEMENTATION
#include "LibGraphics/color/BackgroundScanner.hpp"
#include "modules/stb_image.h"

#include <catch2/catch_test_macros.hpp>
#include <iostream>

using LibGraphics::Color::BackgroundScanner;
using LibGraphics::Image;

// background_color_change_up
TEST_CASE("background_color_change_up: returns -1 if attempts exceeded [ImageWrapper]", "[BackgroundScanner][up]") {
    Image img = Image::load("../tests/assets/strips/up_1x10p8.png");
    REQUIRE(BackgroundScanner::background_color_change_up(img, 0, 9, 2, false) == -1);
}

TEST_CASE("background_color_change_up: returns correctly detected index 1 [ImageWrapper]", "[BackgroundScanner][up]") {
    Image img = Image::load("../tests/assets/strips/up_1x10p1.png");
    REQUIRE(BackgroundScanner::background_color_change_up(img, 0, 9, 10, false) == 1);
}

TEST_CASE("background_color_change_up: returns correctly detected index 4 [ImageWrapper]", "[BackgroundScanner][up]") {
    Image img = Image::load("../tests/assets/strips/up_1x10p8.png");
    REQUIRE(BackgroundScanner::background_color_change_up(img, 0, 9, 10, false) == 8);
}

TEST_CASE("background_color_change_up: returns -1 if there was no color change detected [ImageWrapper]", "[BackgroundScanner][up]") {
    Image img = Image::load("../tests/assets/strips/1x10blank.png");
    REQUIRE(BackgroundScanner::background_color_change_up(img, 0, 9, 15, false) == -1);
}

// background_color_change_down
TEST_CASE("background_color_change_down: returns -1 if attempts exceeded [ImageWrapper]", "[BackgroundScanner][down]") {
    Image img = Image::load("../tests/assets/strips/down_1x10p8.png");
    REQUIRE(BackgroundScanner::background_color_change_down(img, 0, 0, 1, false) == -1);
}

TEST_CASE("background_color_change_down: returns correctly detected index 1 [ImageWrapper]", "[BackgroundScanner][down]") {
    Image img = Image::load("../tests/assets/strips/down_1x10p1.png");
    REQUIRE(BackgroundScanner::background_color_change_down(img, 0, 0, 10, false) == 1);
}

TEST_CASE("background_color_change_down: returns correctly detected index 7 [ImageWrapper]", "[BackgroundScanner][down]") {
    Image img = Image::load("../tests/assets/strips/down_1x10p7.png");
    REQUIRE(BackgroundScanner::background_color_change_down(img, 0, 0, 10, false) == 7);
}

TEST_CASE("background_color_change_down: returns correctly detected index 8 [ImageWrapper]", "[BackgroundScanner][down]") {
    Image img = Image::load("../tests/assets/strips/down_1x10p8.png");
    REQUIRE(BackgroundScanner::background_color_change_down(img, 0, 0, 10, false) == 8);
}

TEST_CASE("background_color_change_down: returns -1 if there was no color change detected [ImageWrapper]", "[BackgroundScanner][down]") {
    Image img = Image::load("../tests/assets/strips/1x10blank.png");
    REQUIRE(BackgroundScanner::background_color_change_down(img, 0, 0, 15, false) == -1);
}

// background_color_change_left
TEST_CASE("background_color_change_left: returns -1 if attempts exceeded [ImageWrapper]", "[BackgroundScanner][left]") {
    Image img = Image::load("../tests/assets/strips/left_10x1p1.png");
    REQUIRE(BackgroundScanner::background_color_change_left(img, 0, 0, 1, false) == -1);
}

TEST_CASE("background_color_change_left: returns correctly detected index 1 [ImageWrapper]", "[BackgroundScanner][left]") {
    Image img = Image::load("../tests/assets/strips/left_10x1p1.png");
    REQUIRE(BackgroundScanner::background_color_change_left(img, 9, 0, 10, false) == 1);
}

TEST_CASE("background_color_change_left: returns correctly detected index 5 [ImageWrapper]", "[BackgroundScanner][left]") {
    Image img = Image::load("../tests/assets/strips/left_10x1p5.png");
    REQUIRE(BackgroundScanner::background_color_change_left(img, 9, 0, 10, false) == 5);
}

TEST_CASE("background_color_change_left: returns -1 if there was no color change detected [ImageWrapper]", "[BackgroundScanner][left]") {
    Image img = Image::load("../tests/assets/strips/10x1blank.png");
    REQUIRE(BackgroundScanner::background_color_change_left(img, 9, 0, 15, false) == -1);
}

// background_color_change_right
TEST_CASE("background_color_change_right: returns -1 if attempts exceeded [ImageWrapper]", "[BackgroundScanner][right]") {
    Image img = Image::load("../tests/assets/strips/right_10x1p4.png");
    REQUIRE(BackgroundScanner::background_color_change_right(img, 0, 0, 1, false) == -1);
}

TEST_CASE("background_color_change_right: returns correctly detected index 1 [ImageWrapper]", "[BackgroundScanner][right]") {
    Image img = Image::load("../tests/assets/strips/right_10x1p1.png");
    REQUIRE(BackgroundScanner::background_color_change_right(img, 0, 0, 10, false) == 1);
}

TEST_CASE("background_color_change_right: returns correctly detected index 4 [ImageWrapper]", "[BackgroundScanner][right]") {
    Image img = Image::load("../tests/assets/strips/right_10x1p4.png");
    REQUIRE(BackgroundScanner::background_color_change_right(img, 0, 0, 10, false) == 4);
}

TEST_CASE("background_color_change_right: returns -1 if there was no color change detected [ImageWrapper]", "[BackgroundScanner][right]") {
    Image img = Image::load("../tests/assets/strips/10x1blank.png");
    REQUIRE(BackgroundScanner::background_color_change_right(img, 0, 0, 15, false) == -1);
}
