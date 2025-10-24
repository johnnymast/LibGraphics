#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#include "LibGraphics/Image.hpp"

#include <opencv2/opencv.hpp>
#include <stdexcept>

namespace LibGraphics {
    Image Image::load(const std::string& path) {
        Image img;

        // Load image using stb_image
        int width, height, channels;
        unsigned char* imageData = stbi_load(path.c_str(), &width, &height, &channels, 0);

        if (!imageData) {
            throw std::runtime_error("Failed to load image: " + path);
        }

        img.width = width;
        img.height = height;
        img.channels = channels;

        // Copy data to vector
        size_t dataSize = static_cast<size_t>(width) * height * channels;
        img.data.assign(imageData, imageData + dataSize);

        // Free stb_image data
        stbi_image_free(imageData);

        return img;
    }

    bool Image::save(const std::string& path, int quality) const {
        if (data.empty() || width <= 0 || height <= 0 || channels <= 0) {
            return false;
        }

        // Determine file format from extension
        std::string ext = path.substr(path.find_last_of(".") + 1);

        int result = 0;
        if (ext == "png" || ext == "PNG") {
            result = stbi_write_png(path.c_str(), width, height, channels, data.data(), width * channels);
        } else if (ext == "jpg" || ext == "jpeg" || ext == "JPG" || ext == "JPEG") {
            result = stbi_write_jpg(path.c_str(), width, height, channels, data.data(), quality);
        } else if (ext == "bmp" || ext == "BMP") {
            result = stbi_write_bmp(path.c_str(), width, height, channels, data.data());
        } else if (ext == "tga" || ext == "TGA") {
            result = stbi_write_tga(path.c_str(), width, height, channels, data.data());
        } else {
            // Default to PNG if unknown extension
            result = stbi_write_png(path.c_str(), width, height, channels, data.data(), width * channels);
        }

        return result != 0;
    }

    Image Image::fromMat(const cv::Mat& mat) {
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

    Image Image::clone() const {
        Image copy = *this;
        return copy;
    }
}