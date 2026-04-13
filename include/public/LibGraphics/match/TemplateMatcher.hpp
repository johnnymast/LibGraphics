#pragma once

#include "LibGraphics/Image.hpp"
#include "LibGraphics/export.hpp"
#include "LibGraphics/match/MatchResult.hpp"
#include "LibGraphics/match/MatchOptions.hpp"

namespace LibGraphics::Match {

    class LIBGRAPHICS_API TemplateMatcher {
    public:

        static MatchResult matchTemplateSingle(
            const Image& match_template,
            const Image& match_target,
            const MatchOptions& options = MatchOptions()
        );

        static std::vector<MatchResult> matchTemplateMultiple(
            const Image& match_template,
            const Image& match_target,
            const MatchOptions& options = MatchOptions()
        );
    };
}