#include "LibGraphics/Image.hpp"

#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include <iostream>

using namespace LibGraphics;

TEST_CASE("Image::load loads valid image", "[Image][load]") {
    const std::string path = "../tests/assets/image/tux.png"; // Make sure this file exists
    REQUIRE(std::filesystem::exists(path));

    Image img = Image::load(path);
    REQUIRE(img.width > 0);
    REQUIRE(img.height > 0);
    REQUIRE(img.channels >= 1);
    REQUIRE(!img.data.empty());
}

TEST_CASE("Image::save writes image to disk", "[Image][save]") {
    Image img;
    img.width = 2;
    img.height = 2;
    img.channels = 3;
    img.data = {
        255, 0, 0,   0, 255, 0,
        0, 0, 255,  255, 255, 0
    };

    const std::string path = "../tests/assets/tmp/test_save.png";
    REQUIRE(img.save(path));
    REQUIRE(std::filesystem::exists(path));

    std::error_code ec;
    std::filesystem::remove(path, ec);
    if (ec) {
        WARN("[Image::show] Failed to delete temp file: " + ec.message());
    }
}

TEST_CASE("Image::crop returns correct region", "[Image][crop]") {
    Image img;
    img.width = 4;
    img.height = 4;
    img.channels = 1;
    img.data = {
        1, 2, 3, 4,
        5, 6, 7, 8,
        9,10,11,12,
       13,14,15,16
    };

    SECTION("Valid crop") {
        Image cropped = img.crop(1, 1, 2, 2);
        REQUIRE(cropped.width == 2);
        REQUIRE(cropped.height == 2);
        REQUIRE(cropped.channels == 1);
        REQUIRE(cropped.data == std::vector<uint8_t>{6, 7, 10, 11});
    }

    SECTION("Out-of-bounds crop returns empty image") {
        Image fail = img.crop(3, 3, 2, 2);
        REQUIRE(fail.width == 0);
        REQUIRE(fail.height == 0);
        REQUIRE(fail.data.empty());
    }
}

TEST_CASE("Image::clone creates deep copy", "[Image][clone]") {
    Image img;
    img.width = 2;
    img.height = 2;
    img.channels = 1;
    img.data = {1, 2, 3, 4};

    Image copy = img.clone();
    REQUIRE(copy.width == img.width);
    REQUIRE(copy.height == img.height);
    REQUIRE(copy.channels == img.channels);
    REQUIRE(copy.data == img.data);

    // Modify original, ensure copy is unaffected
    img.data[0] = 99;
    REQUIRE(copy.data[0] == 1);
}