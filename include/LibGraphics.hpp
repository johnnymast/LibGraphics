#pragma once

#include "generated/version.hpp"
#include "internals/export.hpp"

#include "LibGraphics/match/TemplateMatcher.hpp"
#include "LibGraphics/color/BackgroundScanner.hpp"

namespace LibGraphics {
    LIBGRAPHICS_API struct LibGraphicsInfo {
        int version_major = RELEASE_VERSION_MAJOR;
        int version_minor = RELEASE_VERSION_MINOR;
        int version_patch = RELEASE_VERSION_PATCH;
    };

    LIBGRAPHICS_API LibGraphicsInfo getLibGraphicsVersion();
}