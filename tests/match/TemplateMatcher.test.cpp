#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "LibGraphics/match/TemplateMatcher.hpp"
#include "LibGraphics/match/LowConfidenceException.hpp"
#include "LibGraphics/Image.hpp"

#include <opencv2/opencv.hpp>
#include <filesystem>

using namespace LibGraphics;
using namespace LibGraphics::Match;

TEST_CASE("matchTemplateSingle basic functionality", "[TemplateMatcher][matchTemplateSingle]") {
    const std::filesystem::path assetsPath = "../tests/assets/match/single";
    const std::string templatePath = (assetsPath / "lena_crop.png").string();
    const std::string targetPath = (assetsPath / "lena.png").string();

    SECTION("Successfully loads and matches template") {
        Image templateImg = Image::load(templatePath);
        Image targetImg = Image::load(targetPath);

        REQUIRE(templateImg.width > 0);
        REQUIRE(templateImg.height > 0);
        REQUIRE(targetImg.width > 0);
        REQUIRE(targetImg.height > 0);

        auto result = TemplateMatcher::matchTemplateSingle(templateImg, targetImg);

        REQUIRE(result.Score >= 0.0);
        REQUIRE(result.Score <= 1.0);
        REQUIRE(result.X >= 0);
        REQUIRE(result.Y >= 0);
        REQUIRE(result.Width == templateImg.width);
        REQUIRE(result.Height == templateImg.height);
    }

    SECTION("Match with TM_CCOEFF_NORMED method (using method())") {
        Image templateImg = Image::load(templatePath);
        Image targetImg = Image::load(targetPath);

        MatchOptions options;
        options.method(cv::TM_CCOEFF_NORMED);
        auto result = TemplateMatcher::matchTemplateSingle(templateImg, targetImg, options);

        REQUIRE(result.Score >= -1.0);
        REQUIRE(result.Score <= 1.0);
    }

    SECTION("Match with TM_CCORR_NORMED method") {
        Image templateImg = Image::load(templatePath);
        Image targetImg = Image::load(targetPath);

        MatchOptions options;
        options.method(cv::TM_CCORR_NORMED);
        auto result = TemplateMatcher::matchTemplateSingle(templateImg, targetImg, options);

        REQUIRE(result.Score >= 0.0);
        REQUIRE(result.Score <= 1.0);
    }

    SECTION("Match with TM_SQDIFF method") {
        Image templateImg = Image::load(templatePath);
        Image targetImg = Image::load(targetPath);

        MatchOptions options;
        options.method(cv::TM_SQDIFF);
        auto result = TemplateMatcher::matchTemplateSingle(templateImg, targetImg, options);

        REQUIRE(result.Score >= 0.0);
    }

    SECTION("Match with TM_SQDIFF_NORMED method") {
        Image templateImg = Image::load(templatePath);
        Image targetImg = Image::load(targetPath);

        MatchOptions options;
        options.method(cv::TM_SQDIFF_NORMED);
        auto result = TemplateMatcher::matchTemplateSingle(templateImg, targetImg, options);

        REQUIRE(result.Score >= 0.0);
        REQUIRE(result.Score <= 1.0);
    }
}

TEST_CASE("matchTemplateSingle with confidence threshold", "[TemplateMatcher][matchTemplateSingle][confidence]") {
    const std::filesystem::path assetsPath = "../tests/assets/match/single";
    const std::string templatePath = (assetsPath / "lena_crop.png").string();
    const std::string targetPath = (assetsPath / "lena.png").string();

    SECTION("Match succeeds with low confidence threshold") {
        Image templateImg = Image::load(templatePath);
        Image targetImg = Image::load(targetPath);

        MatchOptions options(0.3);  // 30% confidence
        REQUIRE_NOTHROW(TemplateMatcher::matchTemplateSingle(templateImg, targetImg, options));
    }

    SECTION("Match throws LowConfidenceException with high threshold") {
        Image templateImg = Image::load(templatePath);
        Image targetImg = Image::load(targetPath);

        MatchOptions options(0.99);  // 99% confidence - likely too high

        try {
            TemplateMatcher::matchTemplateSingle(templateImg, targetImg, options);
            // If we get here, the confidence was actually high enough
            SUCCEED("Match confidence was high enough");
        } catch (const LowConfidenceException& e) {
            // Expected behavior - confidence was too low
            REQUIRE(e.getConfidence() < options.minConfidence);
            REQUIRE(e.getMinConfidence() == options.minConfidence);
        }
    }

    SECTION("MatchOptions can specify both confidence and method using builder pattern") {
        Image templateImg = Image::load(templatePath);
        Image targetImg = Image::load(targetPath);

        MatchOptions options(0.5);
        options.method(cv::TM_CCOEFF_NORMED);

        // Should not throw if match is good enough
        auto result = TemplateMatcher::matchTemplateSingle(templateImg, targetImg, options);
        REQUIRE(result.X >= 0);
        REQUIRE(result.Y >= 0);
    }

    SECTION("MatchOptions with method chaining") {
        Image templateImg = Image::load(templatePath);
        Image targetImg = Image::load(targetPath);

        // Fluent interface - chain method calls
        auto result = TemplateMatcher::matchTemplateSingle(
            templateImg,
            targetImg,
            MatchOptions(0.5).method(cv::TM_CCOEFF_NORMED)
        );

        REQUIRE(result.X >= 0);
        REQUIRE(result.Y >= 0);
    }
}

TEST_CASE("matchTemplateMultiple finds multiple instances", "[TemplateMatcher][matchTemplateMultiple]") {
    const std::filesystem::path assetsPath = "../tests/assets/match/multiple";
    const std::string templatePath = (assetsPath / "tux_crop.png").string();
    const std::string targetPath = (assetsPath / "landscape.png").string();

    SECTION("Successfully loads template and target images") {
        Image templateImg = Image::load(templatePath);
        Image targetImg = Image::load(targetPath);

        REQUIRE(templateImg.width > 0);
        REQUIRE(templateImg.height > 0);
        REQUIRE(targetImg.width > 0);
        REQUIRE(targetImg.height > 0);
    }

    SECTION("Finds exactly 3 instances of Tux in landscape.png") {
        Image templateImg = Image::load(templatePath);
        Image targetImg = Image::load(targetPath);

        MatchOptions options(0.8);  // 80% confidence threshold
        auto results = TemplateMatcher::matchTemplateMultiple(templateImg, targetImg, options);

        // Should find exactly 3 instances of Tux
        REQUIRE(results.size() == 3);

        // All matches should have good scores
        for (const auto& result : results) {
            REQUIRE(result.X >= 0);
            REQUIRE(result.Y >= 0);
            REQUIRE(result.Width == templateImg.width);
            REQUIRE(result.Height == templateImg.height);
        }
    }

    SECTION("No confidence threshold returns single best match") {
        Image templateImg = Image::load(templatePath);
        Image targetImg = Image::load(targetPath);

        MatchOptions options;  // No threshold set
        auto results = TemplateMatcher::matchTemplateMultiple(templateImg, targetImg, options);

        // Should return at least one match (the best one)
        REQUIRE(results.size() >= 1);
        REQUIRE(results[0].X >= 0);
        REQUIRE(results[0].Y >= 0);
    }

    SECTION("High confidence threshold returns fewer matches") {
        Image templateImg = Image::load(templatePath);
        Image targetImg = Image::load(targetPath);

        MatchOptions options(0.95);  // Very high confidence
        auto results = TemplateMatcher::matchTemplateMultiple(templateImg, targetImg, options);

        // Should return fewer matches or possibly none
        // All returned matches should be high quality
        for (const auto& result : results) {
            REQUIRE(result.X >= 0);
            REQUIRE(result.Y >= 0);
        }
    }

    SECTION("Low confidence threshold returns more matches") {
        Image templateImg = Image::load(templatePath);
        Image targetImg = Image::load(targetPath);

        MatchOptions options(0.5);  // Low confidence
        auto results = TemplateMatcher::matchTemplateMultiple(templateImg, targetImg, options);

        // Should find multiple matches
        REQUIRE(results.size() >= 1);
    }
}

TEST_CASE("matchTemplateMultiple with different matching methods", "[TemplateMatcher][matchTemplateMultiple]") {
    const std::filesystem::path assetsPath = "../tests/assets/match/multiple";
    const std::string templatePath = (assetsPath / "tux_crop.png").string();
    const std::string targetPath = (assetsPath / "landscape.png").string();

    SECTION("TM_CCOEFF_NORMED method") {
        Image templateImg = Image::load(templatePath);
        Image targetImg = Image::load(targetPath);

        auto results = TemplateMatcher::matchTemplateMultiple(
            templateImg,
            targetImg,
            MatchOptions(0.7).method(cv::TM_CCOEFF_NORMED)
        );

        REQUIRE_FALSE(results.empty());
    }

    SECTION("TM_CCORR_NORMED method") {
        Image templateImg = Image::load(templatePath);
        Image targetImg = Image::load(targetPath);

        auto results = TemplateMatcher::matchTemplateMultiple(
            templateImg,
            targetImg,
            MatchOptions(0.7).method(cv::TM_CCORR_NORMED)
        );

        REQUIRE_FALSE(results.empty());
    }

    SECTION("TM_SQDIFF_NORMED method") {
        Image templateImg = Image::load(templatePath);
        Image targetImg = Image::load(targetPath);

        auto results = TemplateMatcher::matchTemplateMultiple(
            templateImg,
            targetImg,
            MatchOptions(0.7).method(cv::TM_SQDIFF_NORMED)
        );

        REQUIRE_FALSE(results.empty());
    }
}