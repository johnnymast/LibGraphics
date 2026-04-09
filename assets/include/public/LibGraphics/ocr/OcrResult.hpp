#pragma once

#include "LibGraphics/export.hpp"

using LibGraphics::Image;

namespace LibGraphics::Ocr {
    struct LIBGRAPHICS_API OcrResult {
        std::string text;
        double confidence;
    };
}