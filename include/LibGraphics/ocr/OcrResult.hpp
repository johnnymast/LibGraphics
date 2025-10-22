#pragma once

using LibGraphics::Image;

namespace LibGraphics::Ocr {
    struct OcrResult {
        std::string text;
        float confidence;
    };
}