#include "LibGraphics/utils/Converter.hpp"

#include <leptonica/allheaders.h>
#include <stdexcept>

using LibGraphics::Image;

namespace LibGraphics::Utils {

    Pix *Converter::imageToPix(const Image &image) {
        // Leptonica works with 32bpp (RGBA) or 24bpp (RGB) or 8bpp (grayscale)
        int depth = image.channels * 8;

        // Create a Pix structure
        Pix *pix = pixCreate(image.width, image.width, depth);
        if (!pix) {
            return nullptr;
        }

        // Get the raw data from Image
        const uint8_t *srcData = image.data.data();

        // Copy data based on channels
        if (image.channels == 4) {
            // RGBA
            for (int y = 0; y < image.height; y++) {
                for (int x = 0; x < image.width; x++) {
                    int srcIdx = (y * image.width + x) * 4;
                    uint8_t r = srcData[srcIdx];
                    uint8_t g = srcData[srcIdx + 1];
                    uint8_t b = srcData[srcIdx + 2];
                    uint8_t a = srcData[srcIdx + 3];

                    l_uint32 rgba;
                    composeRGBAPixel(r, g, b, a, &rgba);
                    pixSetPixel(pix, x, y, rgba);

                }
            }
        } else if (image.channels== 3) {
            // RGB
            for (int y = 0; y < image.height; y++) {
                for (int x = 0; x < image.width; x++) {
                    int srcIdx = (y * image.width + x) * 3;
                    uint8_t r = srcData[srcIdx];
                    uint8_t g = srcData[srcIdx + 1];
                    uint8_t b = srcData[srcIdx + 2];

                    pixSetRGBPixel(pix, x, y, r, g, b);
                }
            }
        } else if (image.channels== 1) {
            // Grayscale
            for (int y = 0; y < image.height; y++) {
                for (int x = 0; x < image.width; x++) {
                    int srcIdx = y * image.width + x;
                    uint8_t gray = srcData[srcIdx];

                    pixSetPixel(pix, x, y, gray);
                }
            }
        }

        return pix;
    }

    cv::Mat Converter::ImageToMat(const Image& image) {

        if (image.data.empty() || image.width <= 0 || image.height <= 0 || image.channels <= 0) {
            return {};
        }

        int cvType;
        switch (image.channels) {
            case 1:
                cvType = CV_8UC1;
                break;
            case 3:
                cvType = CV_8UC3;
                break;
            case 4:
                cvType = CV_8UC4;
                break;
            default:
                throw std::runtime_error("Unsupported number of channels: " + std::to_string(image.channels));
        }

        cv::Mat mat(image.height, image.width, cvType, const_cast<uint8_t*>(image.data.data()));

        return mat.clone();
    }

    Image Converter::MatToImage(const cv::Mat &mat) {
        Image img;

        if (mat.empty()) {
            throw std::runtime_error("Cannot convert empty cv::Mat to Image");
        }

        img.width = mat.cols;
        img.height = mat.rows;
        img.channels = mat.channels();

        // Convert Mat to continuous if necessary
        cv::Mat continuous = mat.isContinuous() ? mat : mat.clone();

        // Copy data
        size_t dataSize = static_cast<size_t>(img.width) * img.height * img.channels;
        img.data.assign(continuous.data, continuous.data + dataSize);

        return img;
    }
}