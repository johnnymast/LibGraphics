#pragma once

#include "LibGraphics/Image.hpp"
#include "OcrResult.hpp"

namespace LibGraphics::Ocr {
    class OcrTextReader {
    public:
        static std::string CleanOcrOutput(const std::string& input);
        static OcrResult ReadFromImage(const Image& image, const char &language = *"eng");
    };
}

