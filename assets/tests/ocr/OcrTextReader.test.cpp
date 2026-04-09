#include "LibGraphics/ocr/OcrTextReader.hpp"

#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include <iostream>

using namespace LibGraphics::Ocr;
using LibGraphics::Image;

TEST_CASE("OcrTextReader basic functionality", "[OcrTextReader][OcrTextReader]") {
    const std::filesystem::path assetsPath = "../tests/assets/ocr";
    const std::string templatePath = (assetsPath / "ocr-test.png").string();

    OcrTextReader reader;

    SECTION("Read from empty image") {
        const Image emptyImage;
        auto [text, confidence] = reader.ReadFromImage(emptyImage);

        REQUIRE(text.empty());
        REQUIRE(confidence == 0.0f);
    }

    SECTION("Should be able to read text") {
        Image testImage = Image::load("../tests/assets/ocr/ocr-test.png");

        REQUIRE(testImage.width > 0);
        REQUIRE(testImage.height > 0);

        auto [text, confidence] = reader.ReadFromImage(testImage);

        REQUIRE(text == "IF YOU CAN READ THIS THEN THE TEST IS SUCESSFUL");
        REQUIRE(confidence > 0.0f);
    }
}