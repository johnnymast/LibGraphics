#include "LibGraphics/utils/Converter.hpp"
#include "LibGraphics/Image.hpp"

#include <iostream>
#include <leptonica/allheaders.h>
#include <stdexcept>

using LibGraphics::Image;

namespace LibGraphics::Utils {
    Pix *Converter::imageToPix(const Image &image) {
        // Note: RGBA is not supported, instead, conversion to RGB is forced.

        Pix *pix = nullptr;

        if (image.data.empty() || image.width <= 0 || image.height <= 0) {
            throw std::runtime_error("Invalid image dimensions or empty data");
        }

        if (image.channels == 1) {
            pix = pixCreate(image.width, image.height, 8);
            if (!pix) {
                throw std::runtime_error("Failed to create grayscale Pix");
            }

            l_uint8 *pixData = reinterpret_cast<l_uint8 *>(pixGetData(pix));
            int wpl = pixGetWpl(pix);

            for (int y = 0; y < image.height; ++y) {
                const uint8_t *srcRow = &image.data[y * image.width];
                memcpy(pixData + y * wpl * 4, srcRow, image.width);
            }
        } else if (image.channels == 3 || image.channels == 4) {
            std::cout << "[Converter] Processing as RGB" << (image.channels == 4 ? "A" : "") << " (32 bpp)" << std::endl;
            pix = pixCreate(image.width, image.height, 32);
            if (!pix) {
                std::cerr << "[Converter] ERROR: Failed to create RGB Pix" << std::endl;
                throw std::runtime_error("Failed to create RGB Pix");
            }

            l_uint32* pixData = reinterpret_cast<l_uint32*>(pixGetData(pix));
            int wpl = pixGetWpl(pix);

            for (int y = 0; y < image.height; ++y) {
                for (int x = 0; x < image.width; ++x) {
                    int idx = (y * image.width + x) * image.channels;
                    l_uint8 r = image.data[idx];
                    l_uint8 g = image.data[idx + 1];
                    l_uint8 b = image.data[idx + 2];
                    l_uint8 a = image.channels == 4 ? image.data[idx + 3] : 255;

                    // Optional: un-premultiply if needed
                    if (image.channels == 4 && a > 0 && a < 255) {
                        float alphaFactor = a / 255.0f;
                        r = static_cast<l_uint8>(std::min(255.0f, r / alphaFactor));
                        g = static_cast<l_uint8>(std::min(255.0f, g / alphaFactor));
                        b = static_cast<l_uint8>(std::min(255.0f, b / alphaFactor));
                        a = 255; // force full opacity
                    }

                    pixData[y * wpl + x] = (b) | (g << 8) | (r << 16) | (a << 24);
                }
            }
        } else {
            throw std::runtime_error("Unsupported channel count: " + std::to_string(image.channels));
        }

        pixEndianByteSwap(pix);

        return pix;
    }

    cv::Mat Converter::ImageToMat(const Image &image) {
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

        cv::Mat mat(image.height, image.width, cvType, const_cast<uint8_t *>(image.data.data()));

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

        cv::Mat continuous = mat.isContinuous() ? mat : mat.clone();

        size_t dataSize = static_cast<size_t>(img.width) * img.height * img.channels;
        img.data.assign(continuous.data, continuous.data + dataSize);

        return img;
    }
}
