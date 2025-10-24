#include "LibGraphics/Image.hpp"

#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <string>

using namespace LibGraphics;

std::vector<uint8_t> load_file(const std::string& path) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file) {
        throw std::runtime_error("[load_file] Failed to open file: " + path);
    }

    std::streamsize size = file.tellg();
    if (size <= 0) {
        throw std::runtime_error("[load_file] File is empty or unreadable: " + path);
    }

    std::vector<uint8_t> buffer(static_cast<size_t>(size));
    file.seekg(0, std::ios::beg);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        throw std::runtime_error("[load_file] Failed to read file contents: " + path);
    }

    return buffer;
}

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

TEST_CASE("Image::load_from_memory loads valid PNG", "[image][memory]") {
    auto buffer = load_file("../tests/assets/image/tux.png");
    REQUIRE(!buffer.empty());

    Image img = Image::load_from_memory(buffer);

    REQUIRE(img.width > 0);
    REQUIRE(img.height > 0);
    REQUIRE(img.channels >= 1);
    REQUIRE(img.data.size() == img.width * img.height * img.channels);
}

TEST_CASE("Image::load_from_memory throws on corrupt data", "[image][memory][error]") {
    std::vector<uint8_t> corrupt = {0x00, 0x11, 0x22, 0x33, 0x44};

    REQUIRE_THROWS_AS(Image::load_from_memory(corrupt), std::runtime_error);
}

TEST_CASE("Image constructor validates buffer size", "[image][constructor]") {
    int w = 10, h = 10, c = 3;
    std::vector<uint8_t> bad_data(w * h * c - 1); // too small

    REQUIRE_THROWS_AS(Image(w, h, c, std::move(bad_data)), std::invalid_argument);
}

TEST_CASE("Image constructor accepts valid buffer", "[image][constructor]") {
    int w = 10, h = 10, c = 4;
    std::vector<uint8_t> pixels(w * h * c, 255);

    Image img(w, h, c, std::move(pixels));

    REQUIRE(img.width == w);
    REQUIRE(img.height == h);
    REQUIRE(img.channels == c);
    REQUIRE(img.data.size() == w * h * c);
}
