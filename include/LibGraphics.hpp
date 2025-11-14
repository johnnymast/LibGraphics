#pragma once

#include "generated/version.hpp"
#include "internals/export.hpp"

#include "LibGraphics/color/BackgroundScanner.hpp"
#include "LibGraphics/color/Information.hpp"
#include "LibGraphics/match/TemplateMatcher.hpp"
#include "LibGraphics/color/BackgroundScanner.hpp"

#include "LibGraphics/ocr/OcrTextReader.hpp"
#include "LibGraphics/ocr/OcrResult.hpp"

#include "LibGraphics/exceptions/LowConfidenceException.hpp"
#include "LibGraphics/match/TemplateMatcher.hpp"
#include "LibGraphics/Image.hpp"

namespace LibGraphics {
    LIBGRAPHICS_API struct OpenCVInfo {
        int version_major = RELEASE_VERSION_MAJOR;
        int version_minor = RELEASE_VERSION_MINOR;
        int version_patch = RELEASE_VERSION_PATCH;
        bool cuda_supported{false};
    };

    LIBGRAPHICS_API OpenCVInfo GetOpenCVInfo();
}
