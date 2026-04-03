#pragma once

#include "version.hpp"
#include "export.hpp"

#include "color/BackgroundScanner.hpp"
#include "color/Information.hpp"
#include "match/TemplateMatcher.hpp"
#include "color/BackgroundScanner.hpp"

#include "ocr/OcrTextReader.hpp"
#include "ocr/OcrResult.hpp"

#include "exceptions/LowConfidenceException.hpp"
#include "match/TemplateMatcher.hpp"
#include "Image.hpp"

namespace LibGraphics {
    LIBGRAPHICS_API struct OpenCVInfo {
        int version_major = RELEASE_VERSION_MAJOR;
        int version_minor = RELEASE_VERSION_MINOR;
        int version_patch = RELEASE_VERSION_PATCH;
        bool cuda_supported{false};
    };

    LIBGRAPHICS_API OpenCVInfo GetOpenCVInfo();
}
