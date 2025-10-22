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
        OcrResult result = { "", 0.0f};

        try {

            tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();

            if (api->Init(NULL, &language)) {
                fprintf(stderr, "Could not initialize tesseract.\n");
                exit(1);
            }

            Pix *pix = pixRead("../tests/assets/ocr/ocr-test.png");
            // Pix* pix = Converter::imageToPix(image);

            api->SetImage(pix);

            result.text = CleanOcrOutput(std::string(api->GetUTF8Text()));
            result.confidence = api->MeanTextConf();

            api->End();

            delete api;

            pixDestroy(&pix);
        } catch (const std::ios_base::failure &e) {
            return result;
        }

        return result;
    }
} // LibGraphics
