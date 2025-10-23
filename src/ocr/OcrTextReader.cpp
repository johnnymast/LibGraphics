#include "LibGraphics/ocr/OcrTextReader.hpp"
#include "LibGraphics/match/MatchOptions.hpp"
#include "LibGraphics/utils/Converter.hpp"
#include "LibGraphics/Image.hpp"

#include <iostream>
#include <string>
#include <algorithm>

#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>


using LibGraphics::Utils::Converter;

namespace LibGraphics::Ocr {
    std::string OcrTextReader::CleanOcrOutput(const std::string &input) {
        auto start = input.find_first_not_of(" \t\n\r");
        auto end = input.find_last_not_of(" \t\n\r");

        std::string trimmed = (start == std::string::npos) ? "" : input.substr(start, end - start + 1);

        std::replace(trimmed.begin(), trimmed.end(), '\n', ' ');
        std::replace(trimmed.begin(), trimmed.end(), '\r', ' ');

        return trimmed;
    }

    OcrResult OcrTextReader::ReadFromImage(const Image &image, const char &language) {
        OcrResult result = {"", 0.0f};
        Pix *pix = nullptr;
        tesseract::TessBaseAPI *api = nullptr;

        try {
            pix = Converter::imageToPix(image); // may throw

            api = new tesseract::TessBaseAPI();
            if (api->Init(NULL, &language)) {
                delete api;
                pixDestroy(&pix);
                return result;
            }

            api->SetImage(pix);
            result.text = CleanOcrOutput(std::string(api->GetUTF8Text()));
            result.confidence = api->MeanTextConf();

            api->End();
            delete api;
            pixDestroy(&pix);
        } catch (const std::exception &e) {
            if (api) {
                api->End();
                delete api;
            }
            if (pix) pixDestroy(&pix);
            return result;
        }

        return result;
    }
} // LibGraphics
