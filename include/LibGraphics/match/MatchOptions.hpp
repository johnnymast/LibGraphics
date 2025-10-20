#pragma once

#include <opencv2/imgproc.hpp>

namespace LibGraphics::Match {
    struct MatchOptions {
        double minConfidence = 0.0;  // Minimum confidence threshold (0.0 to 1.0)

        MatchOptions() = default;

        explicit MatchOptions(double minConf)
            : minConfidence(minConf), matchMethod_(cv::TM_CCOEFF_NORMED) {}

        // Set the matching method (builder pattern)
        MatchOptions& method(int matchMethod) {
            matchMethod_ = matchMethod;
            return *this;
        }

        // Get the matching method
        int getMethod() const { return matchMethod_; }

    private:
        int matchMethod_ = cv::TM_CCOEFF_NORMED;
    };
}