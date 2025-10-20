#define STB_IMAGE_IMPLEMENTATION
#include "LibGraphics/color/BackgroundScanner.hpp"
#include "modules/stb_image.h"

#include <catch2/catch_test_macros.hpp>
#include <iostream>

using LibGraphics::Color::BackgroundScanner;

/**
 * Note: For those who need to know, the 0,0 is top left. This can cause confusion, hence this comment.
 * Note2: The tests are assumed to be running from the build dir. hence the file paths.
 * Note3: Run All Ctest
 */

// Please note the tests are assumed to be running from the build dir. hence the file paths.

static std::vector<std::vector<uint8_t>> load_grayscale_from_png(const char* path) {

    int w = 0, h = 0, comp = 0;
    unsigned char* data = stbi_load(path, &w, &h, &comp, 1);
    REQUIRE(data != nullptr);

    std::vector<std::vector<uint8_t>> img(h, std::vector<uint8_t>(w, 0));
    for (int y = 0; y < h; ++y) {
        const unsigned char* row = data + y * w;
        for (int x = 0; x < w; ++x) {
            img[y][x] = row[x];
        }
    }
    stbi_image_free(data);
    return img;
}

// background_color_change_up
TEST_CASE("background_color_change_up: returns -1 if attempts exceeded", "[BackgroundScanner][up]") {
    auto img = load_grayscale_from_png("../tests/assets/strips/up_1x10p8.png");
    REQUIRE(BackgroundScanner::background_color_change_up(img, 0, 9, 2, false) == -1);
}

TEST_CASE("background_color_change_up: returns correctly detected index 1", "[BackgroundScanner][up]") {
    auto img = load_grayscale_from_png("../tests/assets/strips/up_1x10p1.png");
    REQUIRE(BackgroundScanner::background_color_change_up(img, 0, 9, 10, false) == 1);
}

TEST_CASE("background_color_change_up: returns correctly detected index 4", "[BackgroundScanner][up]") {
    auto img = load_grayscale_from_png("../tests/assets/strips/up_1x10p8.png");
    REQUIRE(BackgroundScanner::background_color_change_up(img, 0, 9, 10, false) == 8);
}

TEST_CASE("background_color_change_up: returns -1 if there was no color change detected", "[BackgroundScanner][up]") {
    auto img = load_grayscale_from_png("../tests/assets/strips/1x10blank.png");
    REQUIRE(BackgroundScanner::background_color_change_up(img, 0, 9, 15, false) == -1);
}


// background_color_change_down
TEST_CASE("background_color_change_down: returns -1 if attempts exceeded", "[BackgroundScanner][down]") {
    auto img = load_grayscale_from_png("../tests/assets/strips/down_1x10p8.png");
    REQUIRE(BackgroundScanner::background_color_change_down(img, 0, 0, 1, false) == -1);
}

TEST_CASE("background_color_change_down: returns correctly detected index 1", "[BackgroundScanner][down]") {
    auto img = load_grayscale_from_png("../tests/assets/strips/down_1x10p1.png");
    REQUIRE(BackgroundScanner::background_color_change_down(img, 0, 0, 10, false) == 1);
}

TEST_CASE("background_color_change_down: returns correctly detected index 7", "[BackgroundScanner][down]") {
    auto img = load_grayscale_from_png("../tests/assets/strips/down_1x10p7.png");
    REQUIRE(BackgroundScanner::background_color_change_down(img, 0, 0, 10, false) == 7);
}

TEST_CASE("background_color_change_down: returns correctly detected index 8", "[BackgroundScanner][down]") {
    auto img = load_grayscale_from_png("../tests/assets/strips/down_1x10p8.png");
    REQUIRE(BackgroundScanner::background_color_change_down(img, 0, 0, 10, false) == 8);
}

TEST_CASE("background_color_change_up: returns -1 if there was no color change detected", "[BackgroundScanner][down]") {
    auto img = load_grayscale_from_png("../tests/assets/strips/1x10blank.png");
    REQUIRE(BackgroundScanner::background_color_change_down(img, 0, 0, 15, false) == -1);
}


// background_color_change_left
TEST_CASE("background_color_change_left: returns -1 if attempts exceeded", "[BackgroundScanner][left]") {
    auto img = load_grayscale_from_png("../tests/assets/strips/left_10x1p1.png");
    REQUIRE(BackgroundScanner::background_color_change_left(img, 0, 0, 1, false) == -1);
}

TEST_CASE("background_color_change_left: returns correctly detected index 1", "[BackgroundScanner][left]") {
    auto img = load_grayscale_from_png("../tests/assets/strips/left_10x1p1.png");
    REQUIRE(BackgroundScanner::background_color_change_left(img, 9, 0, 10, false) == 1);
}

TEST_CASE("background_color_change_left: returns correctly detected index 5", "[BackgroundScanner][left]") {
    auto img = load_grayscale_from_png("../tests/assets/strips/left_10x1p5.png");
    REQUIRE(BackgroundScanner::background_color_change_left(img, 9, 0, 10, false) == 5);
}

TEST_CASE("background_color_change_left: returns -1 if there was no color change detected", "[BackgroundScanner][left]") {
    auto img = load_grayscale_from_png("../tests/assets/strips/10x1blank.png");
    REQUIRE(BackgroundScanner::background_color_change_left(img, 9, 0, 15, true) == -1);
}


// background_color_change_right
TEST_CASE("background_color_change_right: returns -1 if attempts exceeded", "[BackgroundScanner][right]") {
    auto img = load_grayscale_from_png("../tests/assets/strips/right_10x1p4.png");
    REQUIRE(BackgroundScanner::background_color_change_right(img, 0, 0, 1, false) == -1);
}

TEST_CASE("background_color_change_right: returns correctly detected index 1", "[BackgroundScanner][right]") {
    auto img = load_grayscale_from_png("../tests/assets/strips/right_10x1p1.png");
    REQUIRE(BackgroundScanner::background_color_change_right(img, 0, 0, 10, false) == 1);
}

TEST_CASE("background_color_change_right: returns correctly detected index 4", "[BackgroundScanner][right]") {
    auto img = load_grayscale_from_png("../tests/assets/strips/right_10x1p4.png");
    REQUIRE(BackgroundScanner::background_color_change_right(img, 0, 0, 10, false) == 4);
}

TEST_CASE("background_color_change_right: returns -1 if there was no color change detected", "[BackgroundScanner][right]") {
    auto img = load_grayscale_from_png("../tests/assets/strips/10x1blank.png");
    REQUIRE(BackgroundScanner::background_color_change_right(img, 0, 0, 15, false) == -1);
}