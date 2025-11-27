#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#include "LibGraphics/Image.hpp"
#include "modules/stb_image_write.h"
#include "modules/stb_image.h"

#include <opencv2/opencv.hpp>
#include <stdexcept>
#include <iostream>

namespace LibGraphics {

    // Helper: strip alpha channel
    void Image::stripAlpha(std::vector<uint8_t>& pixels, int width, int height, int& channels) {
        if (channels == 4) {
            std::vector<uint8_t> rgb;
            rgb.reserve(static_cast<size_t>(width) * height * 3);
            for (size_t i = 0; i < pixels.size(); i += 4) {
                rgb.push_back(pixels[i]);     // R
                rgb.push_back(pixels[i + 1]); // G
                rgb.push_back(pixels[i + 2]); // B
            }
            pixels = std::move(rgb);
            channels = 3;
        }
    }

    Image::Image(const int width, const int height, int channels, std::vector<uint8_t> pixels)
           : width(width), height(height), channels(channels) {
        if (width <= 0 || height <= 0 || channels <= 0) {
            throw std::invalid_argument("[Image] Invalid dimensions or channel count");
        }
        if (pixels.size() != static_cast<size_t>(width * height * channels)) {
            throw std::invalid_argument("[Image] Pixel buffer size does not match dimensions");
        }

        // Strip alpha if RGBA
        stripAlpha(pixels, width, height, this->channels);

        origin = "buffer";
        data = std::move(pixels);
    }

    Image Image::load(const std::string &path) {
        int width, height, channels;
        unsigned char *imageData = stbi_load(path.c_str(), &width, &height, &channels, 0);

        if (!imageData) {
            throw std::runtime_error("Failed to load image: " + path);
        }

        size_t dataSize = static_cast<size_t>(width) * height * channels;
        std::vector<uint8_t> pixels(imageData, imageData + dataSize);
        stbi_image_free(imageData);

        // Strip alpha vóór assignment
        stripAlpha(pixels, width, height, channels);

        Image img(width, height, channels, std::move(pixels));
        img.origin = path;
        return img;
    }

    Image Image::load_from_memory(const uint8_t *buffer, const size_t size) {
        int w = 0, h = 0, c = 0;
        stbi_uc *pixelsRaw = stbi_load_from_memory(buffer, static_cast<int>(size), &w, &h, &c, 0);

        if (!pixelsRaw) {
            throw std::runtime_error("[Image::load_from_memory] Failed to decode image from memory");
        }

        std::vector<uint8_t> pixels(pixelsRaw, pixelsRaw + (w * h * c));
        stbi_image_free(pixelsRaw);

        // Strip alpha vóór assignment
        stripAlpha(pixels, w, h, c);

        Image img(w, h, c, std::move(pixels));
        img.origin = "memory";
        return img;
    }

    Image Image::load_from_memory(const std::vector<uint8_t> &buffer) {
        return load_from_memory(buffer.data(), buffer.size());
    }

    bool Image::save(const std::string &path, const int quality) const {
        if (data.empty() || width <= 0 || height <= 0 || channels <= 0) {
            return false;
        }

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
            result = stbi_write_png(path.c_str(), width, height, channels, data.data(), width * channels);
        }

        return result != 0;
    }

    Image Image::crop(const int x, const int y, int width, int height) const {
        if (x < 0 || y < 0 || x + width > this->width || y + height > this->height) {
            std::cerr << "[Image::crop] Invalid crop region\n";
            return Image();
        }

        Image result;
        result.width = width;
        result.height = height;
        result.channels = this->channels;
        result.data.resize(width * height * this->channels);

        for (int row = 0; row < height; ++row) {
            for (int col = 0; col < width; ++col) {
                for (int c = 0; c < this->channels; ++c) {
                    int src_idx = ((y + row) * this->width + (x + col)) * this->channels + c;
                    int dst_idx = (row * width + col) * this->channels + c;
                    result.data[dst_idx] = this->data[src_idx];
                }
            }
        }

        result.origin = origin;
        return result;
    }

    void Image::show() const {
        std::string temp_path = mkTempFilename();

        if (!this->save(temp_path)) {
            std::cerr << "[Image::show] Failed to save temporary image\n";
            return;
        }

#ifdef __linux__
        std::string command = "xdg-open \"" + temp_path + "\"";
        int result = std::system(command.c_str());
        if (result != 0) {
            std::cerr << "[Image::show] Failed to open image with xdg-open\n";
        }
#elif defined(_WIN32)
        ShellExecuteA(nullptr, "open", temp_path.c_str(), nullptr, nullptr, SW_SHOW);
#else
        std::cerr << "[Image::show] Image preview not supported on this platform\n";
#endif

        std::cout << "[Image::show] Press ENTER to continue...\n";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::error_code ec;
        std::filesystem::remove(temp_path, ec);
        if (ec) {
            std::cerr << "[Image::show] Failed to delete temp file: " << ec.message() << "\n";
        }
    }

    Image Image::toGrayscale() const {
        if (width <= 0 || height <= 0 || data.empty()) {
            return Image();
        }

        if (channels == 1) {
            return clone();
        }

        std::vector<uint8_t> grayData;
        grayData.resize(static_cast<size_t>(width) * height);

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                size_t srcIdx = (static_cast<size_t>(y) * width + x) * channels;
                uint8_t r = data[srcIdx + 0];
                uint8_t g = data[srcIdx + 1];
                uint8_t b = data[srcIdx + 2];
                uint8_t gray = static_cast<uint8_t>(
                    static_cast<int>(0.299 * r + 0.587 * g + 0.114 * b)
                );
                grayData[y * width + x] = gray;
            }
        }

        auto clone = Image(width, height, 1, std::move(grayData));
        clone.origin = origin;
        return clone;
    }

    bool Image::isValid() const {
        if (width == 0 || height == 0 || channels == 0) return false;
        const size_t expected = static_cast<size_t>(width) * static_cast<size_t>(height) * static_cast<size_t>(channels);
        return data.size() == expected;
    }

    Image Image::clone() const {
        Image copy = *this;
        return copy;
    }

    std::array<uint8_t, 3> Image::getRGB(int x, int y) const {
        if (x < 0 || x >= width || y < 0 || y >= height) {
            throw std::out_of_range("[Image::getRGB] Coordinates out of bounds");
        }
        if (!isValid()) {
            throw std::runtime_error("[Image::getRGB] Invalid image");
        }

        size_t index = (static_cast<size_t>(y) * width + x) * channels;

        std::array<uint8_t, 3> rgb;
        if (channels == 1) {
            uint8_t gray = data[index];
            rgb = {gray, gray, gray};
        } else if (channels == 3) {
            rgb = {data[index], data[index + 1], data[index + 2]};
        } else {
            throw std::runtime_error("[Image::getRGB] Unsupported channel count");
        }
        return rgb;
    }

    void Image::redact(const Type::Rect& roi, uint8_t value) {
        if (!isValid()) return;
        if (!(channels == 1 || channels == 3)) {
            std::cerr << "Unsupported channel count: " << channels << "\n";
            return;
        }

        int x0 = std::max(0, roi.X);
        int y0 = std::max(0, roi.Y);
        int x1 = std::min(width, roi.X + roi.Width);
        int y1 = std::min(height, roi.Y + roi.Height);

        for (int y = y0; y < y1; ++y) {
            for (int x = x0; x < x1; ++x) {
                size_t idx = (static_cast<size_t>(y) * width + x) * channels;
                if (channels == 1) {
                    data[idx] = value;
                } else {
                    data[idx + 0] = value;
                    data[idx + 1] = value;
                    data[idx + 2] = value;
                }
            }
        }
    }

    void Image::redact(const std::vector<Type::Rect>& rois, uint8_t value) {
        for (const auto& roi : rois) {
            redact(roi, value);
        }
    }
}
