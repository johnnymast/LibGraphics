#pragma once

#include <stdexcept>

namespace LibGraphics::Exceptions {

    class LowConfidenceException : public std::runtime_error {
    public:
        LowConfidenceException(double confidence, double minConfidence)
            : std::runtime_error("Match confidence " + std::to_string(confidence) +
                               " is below minimum threshold " + std::to_string(minConfidence)),
              confidence_(confidence),
              minConfidence_(minConfidence) {}

        double getConfidence() const { return confidence_; }
        double getMinConfidence() const { return minConfidence_; }

    private:
        double confidence_;
        double minConfidence_;
    };
}