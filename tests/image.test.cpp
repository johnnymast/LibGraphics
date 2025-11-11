#include "LibGraphics/Image.hpp"

#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <string>
#include <array>

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

TEST_CASE("Image constructor with invalid buffer size throws", "[Image][constructor]") {
    int w = 4, h = 4, c = 3;
    std::vector<uint8_t> bad_data(w * h * c - 1); // too small
    REQUIRE_THROWS_AS(Image(w, h, c, std::move(bad_data)), std::invalid_argument);
}

TEST_CASE("Image::isValid false for data size mismatch", "[Image][isValid]") {
    // Create a valid image first, then corrupt the buffer size.
    Image img(4, 4, 3, std::vector<uint8_t>(48, 0)); // valid size: 4*4*3 = 48
    img.data.resize(47); // now size does not match width*height*channels
    REQUIRE_FALSE(img.isValid());
}

TEST_CASE("Image::toGrayscale converts RGB to grayscale correctly", "[Image][toGrayscale]") {
    // Prepare a 2x2 RGB image with known pixel values
    // Pixel order: row-major, each pixel is R,G,B
    // Pixel 0: (10, 20, 30)
    // Pixel 1: (40, 50, 60)
    // Pixel 2: (7, 7, 7)
    // Pixel 3: (255, 0, 0)
    std::vector<uint8_t> data = {
        10, 20, 30,
        40, 50, 60,
        7,  7,  7,
        255, 0, 0
    };

    Image src(2, 2, 3, std::move(data));

    Image gray = src.toGrayscale();

    REQUIRE(gray.width == 2);
    REQUIRE(gray.height == 2);
    REQUIRE(gray.channels == 1);
    REQUIRE(gray.data.size() == static_cast<size_t>(2) * 2);

    // Expected grayscale values (rounded to nearest integer):
    // Pixel 0: 0.299*10 + 0.587*20 + 0.114*30 = 18.15 -> 18
    // Pixel 1: 0.299*40 + 0.587*50 + 0.114*60 = 48.15 -> 48
    // Pixel 2: 0.299*7  + 0.587*7  + 0.114*7  = 7.0   -> 7
    // Pixel 3: 0.299*255+0.587*0  + 0.114*0  = 76.245 -> 76

    REQUIRE(gray.data[0] == 18);
    REQUIRE(gray.data[1] == 48);
    REQUIRE(gray.data[2] == 7);
    REQUIRE(gray.data[3] == 76);
}

TEST_CASE("Image::getRGB returns correct pixel values", "[image][getRGB]") {
    SECTION("RGB image returns correct values") {
        // Create a 2x2 RGB image with known pixel values
        std::vector<uint8_t> data = {
            255, 0, 0,     // Red pixel at (0,0)
            0, 255, 0,     // Green pixel at (1,0)
            0, 0, 255,     // Blue pixel at (0,1)
            128, 128, 128  // Gray pixel at (1,1)
        };
        LibGraphics::Image img(2, 2, 3, data);

        auto pixel1 = img.getRGB(0, 0);
        REQUIRE(pixel1[0] == 255);
        REQUIRE(pixel1[1] == 0);
        REQUIRE(pixel1[2] == 0);

        auto pixel2 = img.getRGB(1, 0);
        REQUIRE(pixel2[0] == 0);
        REQUIRE(pixel2[1] == 255);
        REQUIRE(pixel2[2] == 0);

        auto pixel3 = img.getRGB(0, 1);
        REQUIRE(pixel3[0] == 0);
        REQUIRE(pixel3[1] == 0);
        REQUIRE(pixel3[2] == 255);

        auto pixel4 = img.getRGB(1, 1);
        REQUIRE(pixel4[0] == 128);
        REQUIRE(pixel4[1] == 128);
        REQUIRE(pixel4[2] == 128);
    }

    SECTION("RGBA image returns correct RGB values ignoring alpha") {
        // Create a 2x2 RGBA image
        std::vector<uint8_t> data = {
            255, 0, 0, 255,    // Red pixel with full opacity at (0,0)
            0, 255, 0, 128,    // Green pixel with half opacity at (1,0)
            0, 0, 255, 64,     // Blue pixel with low opacity at (0,1)
            255, 255, 255, 0   // White pixel with zero opacity at (1,1)
        };
        LibGraphics::Image img(2, 2, 4, data);

        auto pixel1 = img.getRGB(0, 0);
        REQUIRE(pixel1[0] == 255);
        REQUIRE(pixel1[1] == 0);
        REQUIRE(pixel1[2] == 0);

        auto pixel2 = img.getRGB(1, 0);
        REQUIRE(pixel2[0] == 0);
        REQUIRE(pixel2[1] == 255);
        REQUIRE(pixel2[2] == 0);

        auto pixel4 = img.getRGB(1, 1);
        REQUIRE(pixel4[0] == 255);
        REQUIRE(pixel4[1] == 255);
        REQUIRE(pixel4[2] == 255);
    }

    SECTION("Grayscale image returns same value for all channels") {
        std::vector<uint8_t> data = {
            0, 128, 255, 64
        };
        LibGraphics::Image img(2, 2, 1, data);

        auto pixel1 = img.getRGB(0, 0);
        REQUIRE(pixel1[0] == 0);
        REQUIRE(pixel1[1] == 0);
        REQUIRE(pixel1[2] == 0);

        auto pixel2 = img.getRGB(1, 0);
        REQUIRE(pixel2[0] == 128);
        REQUIRE(pixel2[1] == 128);
        REQUIRE(pixel2[2] == 128);

        auto pixel3 = img.getRGB(0, 1);
        REQUIRE(pixel3[0] == 255);
        REQUIRE(pixel3[1] == 255);
        REQUIRE(pixel3[2] == 255);
    }

    SECTION("Throws exception for out of bounds coordinates") {
        std::vector<uint8_t> data = {
            255, 0, 0,
            0, 255, 0
        };
        LibGraphics::Image img(1, 2, 3, data);

        REQUIRE_THROWS_AS(img.getRGB(-1, 0), std::out_of_range);
        REQUIRE_THROWS_AS(img.getRGB(0, -1), std::out_of_range);
        REQUIRE_THROWS_AS(img.getRGB(1, 0), std::out_of_range);
        REQUIRE_THROWS_AS(img.getRGB(0, 2), std::out_of_range);
        REQUIRE_THROWS_AS(img.getRGB(10, 10), std::out_of_range);
    }

    SECTION("Works with corner and edge pixels") {
        std::vector<uint8_t> data(3 * 3 * 3, 0);
        // Set corner pixels to different values
        // Top-left
        data[0] = 10; data[1] = 20; data[2] = 30;
        // Top-right
        data[6] = 40; data[7] = 50; data[8] = 60;
        // Bottom-left
        data[18] = 70; data[19] = 80; data[20] = 90;
        // Bottom-right
        data[24] = 100; data[25] = 110; data[26] = 120;

        LibGraphics::Image img(3, 3, 3, data);

        auto topLeft = img.getRGB(0, 0);
        REQUIRE(topLeft[0] == 10);
        REQUIRE(topLeft[1] == 20);
        REQUIRE(topLeft[2] == 30);

        auto topRight = img.getRGB(2, 0);
        REQUIRE(topRight[0] == 40);
        REQUIRE(topRight[1] == 50);
        REQUIRE(topRight[2] == 60);

        auto bottomLeft = img.getRGB(0, 2);
        REQUIRE(bottomLeft[0] == 70);
        REQUIRE(bottomLeft[1] == 80);
        REQUIRE(bottomLeft[2] == 90);

        auto bottomRight = img.getRGB(2, 2);
        REQUIRE(bottomRight[0] == 100);
        REQUIRE(bottomRight[1] == 110);
        REQUIRE(bottomRight[2] == 120);
    }
}

TEST_CASE("Redact a single region to black", "[redact]") {
    std::vector<uint8_t> pixels(4 * 4 * 3, 255); // 4x4 white RGB image
    Image img(4, 4, 3, pixels);

    Cordinate region{1, 1, 2, 2};
    img.redact(region);

    SECTION("Pixels inside the region are black") {
        for (int y = 1; y < 3; ++y) {
            for (int x = 1; x < 3; ++x) {
                auto rgb = img.getRGB(x, y);
                REQUIRE(rgb[0] == 0);
                REQUIRE(rgb[1] == 0);
                REQUIRE(rgb[2] == 0);
            }
        }
    }

    SECTION("Pixels outside the region remain white") {
        auto rgb = img.getRGB(0, 0);
        REQUIRE(rgb[0] == 255);
        REQUIRE(rgb[1] == 255);
        REQUIRE(rgb[2] == 255);
    }
}

TEST_CASE("Redact multiple regions to gray", "[redact]") {
    std::vector<uint8_t> pixels(4 * 4 * 3, 255); // 4x4 white RGB image
    Image img(4, 4, 3, pixels);

    std::vector<Cordinate> regions = {
        {0, 0, 2, 2},
        {2, 2, 2, 2}
    };

    img.redact(regions, 128);

    SECTION("First region is gray") {
        for (int y = 0; y < 2; ++y) {
            for (int x = 0; x < 2; ++x) {
                auto rgb = img.getRGB(x, y);
                REQUIRE(rgb[0] == 128);
                REQUIRE(rgb[1] == 128);
                REQUIRE(rgb[2] == 128);
            }
        }
    }

    SECTION("Second region is gray") {
        for (int y = 2; y < 4; ++y) {
            for (int x = 2; x < 4; ++x) {
                auto rgb = img.getRGB(x, y);
                REQUIRE(rgb[0] == 128);
                REQUIRE(rgb[1] == 128);
                REQUIRE(rgb[2] == 128);
            }
        }
    }

    SECTION("Pixels outside all regions remain white") {
        auto rgb = img.getRGB(3, 0);
        REQUIRE(rgb[0] == 255);
        REQUIRE(rgb[1] == 255);
        REQUIRE(rgb[2] == 255);
    }
}

TEST_CASE("Redact region that exceeds image bounds", "[redact]") {
    std::vector<uint8_t> pixels(4 * 4 * 3, 255); // 4x4 white RGB image
    Image img(4, 4, 3, pixels);

    Cordinate region{-5, -5, 10, 10}; // covers entire image
    img.redact(region);

    SECTION("All pixels are black") {
        for (int y = 0; y < 4; ++y) {
            for (int x = 0; x < 4; ++x) {
                auto rgb = img.getRGB(x, y);
                REQUIRE(rgb[0] == 0);
                REQUIRE(rgb[1] == 0);
                REQUIRE(rgb[2] == 0);
            }
        }
    }
}

TEST_CASE("Redact with empty region list does nothing", "[redact]") {
    std::vector<uint8_t> pixels(4 * 4 * 3, 255); // 4x4 white RGB image
    Image img(4, 4, 3, pixels);

    std::vector<Cordinate> empty;
    img.redact(empty);

    SECTION("All pixels remain white") {
        for (int y = 0; y < 4; ++y) {
            for (int x = 0; x < 4; ++x) {
                auto rgb = img.getRGB(x, y);
                REQUIRE(rgb[0] == 255);
                REQUIRE(rgb[1] == 255);
                REQUIRE(rgb[2] == 255);
            }
        }
    }
}