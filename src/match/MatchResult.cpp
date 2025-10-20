#include "LibGraphics/match/MatchResult.hpp"

// rename to MatchResult
namespace LibGraphics::Match {
    MatchResult MatchResult::Center() const
    {
        const int centerX = X + Width / 2;
        const int centerY = Y + Height / 2;
        return MatchResult(centerX, centerY, Width, Height);
    }
}