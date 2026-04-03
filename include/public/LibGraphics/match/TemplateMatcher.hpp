#pragma once

#include "LibGraphics/Image.hpp"
#include "LibGraphics/match/MatchResult.hpp"
#include "LibGraphics/match/MatchOptions.hpp"

namespace LibGraphics::Match {

    class TemplateMatcher {
    public:
        // Find single best match in target image
        static MatchResult matchTemplateSingle(
            const Image& match_template,
            const Image& match_target,
            const MatchOptions& options = MatchOptions()
        );

        // Find all occurrences of template in target image above confidence threshold
        static std::vector<MatchResult> matchTemplateMultiple(
            const Image& match_template,
            const Image& match_target,
            const MatchOptions& options = MatchOptions()
        );
    };
}