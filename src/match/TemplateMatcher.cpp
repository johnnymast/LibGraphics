#include "LibGraphics/match/TemplateMatcher.hpp"
#include "LibGraphics/match/LowConfidenceException.hpp"
#include "LibGraphics/match/MatchOptions.hpp"

#include <LibGraphics/helpers/OpenCV.hpp>
#include "LibGraphics/Image.hpp"

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <algorithm>

using LibGraphics::Helpers::OpenCv;
using LibGraphics::Match::TemplateMatcher;
using LibGraphics::Match::MatchResult;
using LibGraphics::Match::MatchOptions;
using LibGraphics::Match::LowConfidenceException;

// Helper function to normalize score based on match method
static double normalizeScore(double score, int matchMethod) {
    // For SQDIFF methods, lower is better, so invert the score
    if (matchMethod == cv::TM_SQDIFF || matchMethod == cv::TM_SQDIFF_NORMED) {
        return 1.0 - score;  // Convert to higher-is-better
    }
    return score;
}

// Helper function to ensure images have compatible formats for template matching
static void ensureCompatibleFormats(cv::Mat& query, cv::Mat& target) {
    // Both images must have the same depth (8-bit or 32-bit float)
    if (query.depth() != target.depth()) {
        if (query.depth() == CV_8U && target.depth() == CV_32F) {
            query.convertTo(query, CV_32F);
        } else if (query.depth() == CV_32F && target.depth() == CV_8U) {
            target.convertTo(target, CV_32F);
        }
    }

    // Both images must have the same number of channels
    if (query.channels() != target.channels()) {
        if (query.channels() == 4 && target.channels() == 3) {
            // Convert query from BGRA to BGR
            cv::cvtColor(query, query, cv::COLOR_BGRA2BGR);
        } else if (query.channels() == 3 && target.channels() == 4) {
            // Convert target from BGRA to BGR
            cv::cvtColor(target, target, cv::COLOR_BGRA2BGR);
        } else if (query.channels() == 4 && target.channels() == 1) {
            // Convert query from BGRA to grayscale
            cv::cvtColor(query, query, cv::COLOR_BGRA2GRAY);
        } else if (query.channels() == 1 && target.channels() == 4) {
            // Convert target from BGRA to grayscale
            cv::cvtColor(target, target, cv::COLOR_BGRA2GRAY);
        } else if (query.channels() == 3 && target.channels() == 1) {
            // Convert query from BGR to grayscale
            cv::cvtColor(query, query, cv::COLOR_BGR2GRAY);
        } else if (query.channels() == 1 && target.channels() == 3) {
            // Convert target from BGR to grayscale
            cv::cvtColor(target, target, cv::COLOR_BGR2GRAY);
        }
    }
}

// Main implementation with options
MatchResult TemplateMatcher::matchTemplateSingle(
    const Image& query,
    const Image& target,
    const MatchOptions& options
) {
    cv::Mat queryMat = OpenCv::ImageToMat(query);
    cv::Mat targetMat = OpenCv::ImageToMat(target);

    // Ensure compatible formats
    ensureCompatibleFormats(queryMat, targetMat);

    cv::Mat result;

    if (targetMat.cols < queryMat.cols || targetMat.rows < queryMat.rows) {
        throw std::runtime_error("Target image is smaller than query image.");
    }

    int matchMethod = options.getMethod();
    cv::matchTemplate(targetMat, queryMat, result, matchMethod);
    double minVal, maxVal;
    cv::Point minLoc, maxLoc;
    cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);

    double score = (matchMethod == cv::TM_SQDIFF || matchMethod == cv::TM_SQDIFF_NORMED)
                       ? minVal
                       : maxVal;
    cv::Point matchLoc = (matchMethod == cv::TM_SQDIFF || matchMethod == cv::TM_SQDIFF_NORMED)
                             ? minLoc
                             : maxLoc;

    // Check minimum confidence
    if (options.minConfidence > 0.0) {
        double normalizedScore = normalizeScore(score, matchMethod);
        if (normalizedScore < options.minConfidence) {
            throw LowConfidenceException(normalizedScore, options.minConfidence);
        }
    }

    return MatchResult(matchLoc.x, matchLoc.y, queryMat.cols, queryMat.rows, score);
}

// Find all occurrences above threshold
std::vector<MatchResult> TemplateMatcher::matchTemplateMultiple(
    const Image& query,
    const Image& target,
    const MatchOptions& options
) {
    std::vector<MatchResult> results;
    cv::Mat queryMat = OpenCv::ImageToMat(query);
    cv::Mat targetMat = OpenCv::ImageToMat(target);

    // Ensure compatible formats
    ensureCompatibleFormats(queryMat, targetMat);

    cv::Mat result;

    if (targetMat.cols < queryMat.cols || targetMat.rows < queryMat.rows) {
        throw std::runtime_error("Target image is smaller than query image.");
    }

    int matchMethod = options.getMethod();
    cv::matchTemplate(targetMat, queryMat, result, matchMethod);

    // For SQDIFF methods, good matches have low values
    bool invertThreshold = (matchMethod == cv::TM_SQDIFF || matchMethod == cv::TM_SQDIFF_NORMED);

    // Use minConfidence as threshold (default 0.0 means find all matches)
    double threshold = options.minConfidence;

    // If no confidence threshold set, just return the best match
    if (threshold <= 0.0) {
        double minVal, maxVal;
        cv::Point minLoc, maxLoc;
        cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);

        double score = invertThreshold ? minVal : maxVal;
        cv::Point matchLoc = invertThreshold ? minLoc : maxLoc;

        results.push_back(MatchResult(matchLoc.x, matchLoc.y, queryMat.cols, queryMat.rows, score));
        return results;
    }

    // Find all matches above threshold with non-maximum suppression
    int windowSize = std::max(queryMat.cols, queryMat.rows) / 4; // Suppression window

    // Create a copy for marking processed areas
    cv::Mat resultCopy = result.clone();

    while (true) {
        double minVal, maxVal;
        cv::Point minLoc, maxLoc;
        cv::minMaxLoc(resultCopy, &minVal, &maxVal, &minLoc, &maxLoc);

        double score = invertThreshold ? minVal : maxVal;
        cv::Point matchLoc = invertThreshold ? minLoc : maxLoc;

        // Check if this match meets the threshold
        double normalizedScore = normalizeScore(score, matchMethod);

        if (normalizedScore < threshold) {
            break; // No more matches above threshold
        }

        // Add this match
        results.push_back(MatchResult(matchLoc.x, matchLoc.y, queryMat.cols, queryMat.rows, score));

        // Suppress nearby matches to avoid duplicates
        int x1 = std::max(0, matchLoc.x - windowSize);
        int y1 = std::max(0, matchLoc.y - windowSize);
        int x2 = std::min(resultCopy.cols, matchLoc.x + windowSize);
        int y2 = std::min(resultCopy.rows, matchLoc.y + windowSize);

        // Set the suppression area to worst possible value
        cv::Rect suppressRect(x1, y1, x2 - x1, y2 - y1);
        if (invertThreshold) {
            resultCopy(suppressRect).setTo(std::numeric_limits<float>::max());
        } else {
            resultCopy(suppressRect).setTo(std::numeric_limits<float>::lowest());
        }
    }

    return results;
}