#pragma once

#include "LibGraphics/export.hpp"

namespace LibGraphics::Match {
    class LIBGRAPHICS_API MatchResult {
    public:
        int X = 0;
        int Y = 0;
        int Width = 0;
        int Height = 0;
        double Score = 0.0f;

        explicit MatchResult(
            const int x = 0,
            const int y = 0,
            const int width = 0,
            const int height = 0,
            const double score = 0.0f): X(x), Y(y), Width(width), Height(height), Score(score) {}

        [[nodiscard]] MatchResult Center() const;
    };
}