#include "LibGraphics/utils/Converter.hpp"
#include "LibGraphics/Image.hpp"

#include <iostream>
#include <leptonica/allheaders.h>
#include <opencv2/imgproc.hpp>
#include <stdexcept>

using LibGraphics::Image;

namespace LibGraphics::Utils {

    Pix *Converter::imageToPix(Image &image) {
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

            pix = pixCreate(image.width, image.height, 32);
            if (!pix) {
                throw std::runtime_error("Failed to create RGB Pix");
            }

            l_uint32 *pixData = reinterpret_cast<l_uint32 *>(pixGetData(pix));
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

    Image Converter::pixToImage(Pix* pix) {
        int width = pixGetWidth(pix);
        int height = pixGetHeight(pix);
        int depth = pixGetDepth(pix);

        Image img;
        img.width = width;
        img.height = height;

        if (depth == 8) {
            img.channels = 1;
            img.data.resize(width * height);

            for (int y = 0; y < height; ++y) {
                for (int x = 0; x < width; ++x) {
                    l_uint32 pixel;
                    pixGetPixel(pix, x, y, &pixel);
                    img.data[y * width + x] = static_cast<uint8_t>(pixel);
                }
            }
        } else if (depth == 32) {
            img.channels = 4;
            img.data.resize(width * height * 4);

            for (int y = 0; y < height; ++y) {
                for (int x = 0; x < width; ++x) {
                    l_uint32 pixel;
                    pixGetPixel(pix, x, y, &pixel);

                    img.data[(y * width + x) * 4 + 0] = pixel & 0xFF;         // R
                    img.data[(y * width + x) * 4 + 1] = (pixel >> 8) & 0xFF;  // G
                    img.data[(y * width + x) * 4 + 2] = (pixel >> 16) & 0xFF; // B
                    img.data[(y * width + x) * 4 + 3] = (pixel >> 24) & 0xFF; // A
                }
            }
        } else {
            throw std::runtime_error("Unsupported Pix depth");
        }

        return img;
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
                throw std::runtime_error(
                    "LibScreenshot::ImageToMat - unsupported channel count: " + std::to_string(image.channels));
        }

        // Wrap raw data into a Mat header (no copy yet)
        cv::Mat mat(image.height, image.width, cvType, const_cast<uint8_t *>(image.data.data()));

        // Convert RGB to BGR if needed
        if (image.channels == 3) {
            cv::Mat converted;
            cv::cvtColor(mat, converted, cv::COLOR_RGB2BGR);
            return converted;
        }

        // Clone to ensure memory safety for 1 or 4 channels
        return mat.clone();
    }

    Image Converter::MatToImage(const cv::Mat &mat) {
        if (mat.empty()) {
            throw std::runtime_error("LibScreenshot::MatToImage - input Mat is empty");
        }

        Image image;
        image.width = mat.cols;
        image.height = mat.rows;
        image.channels = mat.channels();

        cv::Mat converted;

        switch (mat.channels()) {
            case 1: {
                // Grayscale — clone if non-continuous
                converted = mat.isContinuous() ? mat : mat.clone();
                break;
            }
            case 3: {
                // BGR → RGB — clone before conversion if needed
                cv::Mat temp = mat.isContinuous() ? mat : mat.clone();
                cv::cvtColor(temp, converted, cv::COLOR_BGR2RGB);
                break;
            }
            case 4: {
                // BGRA → RGBA — clone before conversion if needed
                cv::Mat temp = mat.isContinuous() ? mat : mat.clone();
                cv::cvtColor(temp, converted, cv::COLOR_BGRA2RGBA);
                break;
            }
            default:
                throw std::runtime_error("LibScreenshot::MatToImage - unsupported format: " +
                                         std::to_string(mat.cols) + "x" + std::to_string(mat.rows) +
                                         ", type=" + std::to_string(mat.type()));
        }

        image.data.assign(converted.data, converted.data + converted.total() * converted.elemSize());
        return image;
    }
}
