#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define _CRT_SECURE_NO_WARNINGS

#include "LibGraphics/Image.hpp"
#include "LibGraphics/modules/stb_image_write.hpp"
#include "LibGraphics/modules/stb_image.hpp"

#include <opencv2/opencv.hpp>
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <filesystem>
#include <limits>

#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#include <shellapi.h>
#endif

namespace LibGraphics {

    // Remove alpha channel if present
    void Image::stripAlpha(std::vector<uint8_t>& pixels, int width, int height, int& channels) {
        if (channels == 4) {
            std::vector<uint8_t> rgb;
            rgb.reserve(static_cast<size_t>(width) * height * 3);

            for (size_t i = 0; i < pixels.size(); i += 4) {
                rgb.push_back(pixels[i + 0]);
                rgb.push_back(pixels[i + 1]);
                rgb.push_back(pixels[i + 2]);
            }

            pixels = std::move(rgb);
            channels = 3;
        }
    }

    // The ONLY constructor declared in the header
    Image::Image(const int width, const int height, const int channels, std::vector<uint8_t> pixels): width(width), height(height), channels(channels)
    {
        if (width <= 0 || height <= 0 || channels <= 0) {
            throw std::invalid_argument("[Image] Invalid dimensions or channel count");
        }

        if (pixels.size() != static_cast<size_t>(width * height * channels)) {
            throw std::invalid_argument("[Image] Pixel buffer size does not match dimensions");
        }

        stripAlpha(pixels, width, height, this->channels);

        data = std::move(pixels);
        origin = "buffer";
    }

    Image Image::load(const std::string &path) {
        int w, h, c;
        unsigned char* raw = stbi_load(path.c_str(), &w, &h, &c, 0);

        if (!raw) {
            throw std::runtime_error("Failed to load image: " + path);
        }

        std::vector<uint8_t> pixels(raw, raw + (w * h * c));
        stbi_image_free(raw);

        stripAlpha(pixels, w, h, c);

        Image img(w, h, c, std::move(pixels));
        img.origin = path;
        return img;
    }

    Image Image::load_from_memory(const uint8_t* buffer, size_t size) {
        int w = 0, h = 0, c = 0;
        stbi_uc* raw = stbi_load_from_memory(buffer, static_cast<int>(size), &w, &h, &c, 0);

        if (!raw) {
            throw std::runtime_error("[Image::load_from_memory] Failed to decode image");
        }

        std::vector<uint8_t> pixels(raw, raw + (w * h * c));
        stbi_image_free(raw);

        stripAlpha(pixels, w, h, c);

        Image img(w, h, c, std::move(pixels));
        img.origin = "memory";
        return img;
    }

    Image Image::load_from_memory(const std::vector<uint8_t>& buffer) {
        return load_from_memory(buffer.data(), buffer.size());
    }

    bool Image::save(const std::string &path, int quality) const {
        if (!isValid()) return false;

        std::string ext = path.substr(path.find_last_of('.') + 1);
        int result = 0;

        if (ext == "png" || ext == "PNG") {
            result = stbi_write_png(path.c_str(), width, height, channels, data.data(), width * channels);
        }
        else if (ext == "jpg" || ext == "jpeg" || ext == "JPG" || ext == "JPEG") {
            result = stbi_write_jpg(path.c_str(), width, height, channels, data.data(), quality);
        }
        else if (ext == "bmp" || ext == "BMP") {
            result = stbi_write_bmp(path.c_str(), width, height, channels, data.data());
        }
        else if (ext == "tga" || ext == "TGA") {
            result = stbi_write_tga(path.c_str(), width, height, channels, data.data());
        }
        else {
            result = stbi_write_png(path.c_str(), width, height, channels, data.data(), width * channels);
        }

        return result != 0;
    }

    Image Image::crop(int x, int y, int w, int h) const {
        if (x < 0 || y < 0 || x + w > width || y + h > height) {
            std::cerr << "[Image::crop] Invalid region\n";
            return Image();
        }

        Image out;
        out.width = w;
        out.height = h;
        out.channels = channels;
        out.data.resize(static_cast<size_t>(w) * h * channels);

        for (int row = 0; row < h; ++row) {
            for (int col = 0; col < w; ++col) {
                for (int c = 0; c < channels; ++c) {
                    size_t src = ((y + row) * width + (x + col)) * channels + c;
                    size_t dst = (row * w + col) * channels + c;
                    out.data[dst] = data[src];
                }
            }
        }

        out.origin = origin;
        return out;
    }

    void Image::show() const {
        std::string temp = mkTempFilename();

        if (!save(temp)) {
            std::cerr << "[Image::show] Failed to save temp file\n";
            return;
        }

#ifdef __linux__
        std::system(("xdg-open \"" + temp + "\"").c_str());
#elif defined(_WIN32)
        ShellExecuteW(nullptr, L"open",
            std::filesystem::path(temp).wstring().c_str(),
            nullptr, nullptr, SW_SHOW);
#endif

        std::cout << "[Image::show] Press ENTER to continue...\n";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::error_code ec;
        std::filesystem::remove(temp, ec);
    }

    Image Image::toGrayscale() const {
        if (!isValid()) return Image();
        if (channels == 1) return clone();

        std::vector<uint8_t> gray(width * height);

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                size_t idx = (static_cast<size_t>(y) * width + x) * channels;
                uint8_t r = data[idx + 0];
                uint8_t g = data[idx + 1];
                uint8_t b = data[idx + 2];

                gray[y * width + x] = static_cast<uint8_t>(
                    0.299 * r + 0.587 * g + 0.114 * b
                );
            }
        }

        Image out(width, height, 1, std::move(gray));
        out.origin = origin;
        return out;
    }

    bool Image::isValid() const {
        if (width <= 0 || height <= 0 || channels <= 0) return false;
        return data.size() == static_cast<size_t>(width) * height * channels;
    }

    Image Image::clone() const {
        Image copy = *this;
        return copy;
    }

    std::array<uint8_t, 3> Image::getRGB(int x, int y) const {
        if (!isValid()) throw std::runtime_error("[Image::getRGB] Invalid image");
        if (x < 0 || x >= width || y < 0 || y >= height)
            throw std::out_of_range("[Image::getRGB] Out of bounds");

        size_t idx = (static_cast<size_t>(y) * width + x) * channels;

        if (channels == 1) {
            uint8_t g = data[idx];
            return {g, g, g};
        }

        return {data[idx], data[idx + 1], data[idx + 2]};
    }

    void Image::redact(const Type::Rect& roi, uint8_t value) {
        if (!isValid()) return;

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
        for (const auto& r : rois) redact(r, value);
    }
}
