#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#include "LibGraphics/Image.hpp"
#include "modules/stb_image_write.h"
#include "modules/stb_image.h"

#include <opencv2/opencv.hpp>
#include <stdexcept>

namespace LibGraphics {
    // Image::Image(int width, int height, int channels, std::vector<uint8_t> pixels)
    //     : width(width), height(height), channels(channels) {
    //     if (width <= 0 || height <= 0 || channels <= 0) {
    //         throw std::invalid_argument("[Image] Invalid dimensions or channel count");
    //     }
    //     if (pixels.size() != static_cast<size_t>(width * height * channels)) {
    //         throw std::invalid_argument("[Image] Pixel buffer size does not match dimensions");
    //     }
    //     data = std::move(pixels);
    // }

    Image::Image(const int width, const int height, int channels, std::vector<uint8_t> pixels)
           : width(width), height(height), channels(channels) {
        if (width <= 0 || height <= 0 || channels <= 0) {
            throw std::invalid_argument("[Image] Invalid dimensions or channel count");
        }
        if (pixels.size() != static_cast<size_t>(width * height * channels)) {
            throw std::invalid_argument("[Image] Pixel buffer size does not match dimensions");
        }

        origin = "buffer";
        data = std::move(pixels);
    }


    Image Image::load(const std::string &path) {
        Image img;

        int width, height, channels;
        unsigned char *imageData = stbi_load(path.c_str(), &width, &height, &channels, 0);

        if (!imageData) {
            throw std::runtime_error("Failed to load image: " + path);
        }

        img.width = width;
        img.height = height;
        img.channels = channels;
        img.origin = path;

        size_t dataSize = static_cast<size_t>(width) * height * channels;
        img.data.assign(imageData, imageData + dataSize);

        stbi_image_free(imageData);

        return img;
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
        // Validate bounds
        if (x < 0 || y < 0 || x + width > this->width || y + height > this->height) {
            std::cerr << "[Image::crop] Invalid crop region\n";
            return Image(); // or throw, or return empty
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

        // Step 2: Launch viewer
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

        // Wait for keypress
        std::cout << "[Image::show] Press ENTER to continue...\n";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // Delete temp file
        std::error_code ec;
        std::filesystem::remove(temp_path, ec);
        if (ec) {
            std::cerr << "[Image::show] Failed to delete temp file: " << ec.message() << "\n";
        }
    }

    /**
     * Convert the image to grayscale and return a new 1-channel image.
     * If the source already has a single channel, returns a clone.
     * The resulting image uses 8 bits per pixel (grayscale).
     */
    Image Image::toGrayscale() const {
        // Basic validation
        if (width <= 0 || height <= 0 || data.empty()) {
            return Image();
        }

        // If already grayscale, return a clone
        if (channels == 1) {
            return clone();
        }

        // Allocate grayscale data: width * height, 1 channel
        std::vector<uint8_t> grayData;
        grayData.resize(static_cast<size_t>(width) * height);

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                size_t srcIdx = (static_cast<size_t>(y) * width + x) * channels;
                uint8_t r = data[srcIdx + 0];
                uint8_t g = data[srcIdx + 1];
                uint8_t b = data[srcIdx + 2];
                // Rec. 601 luma: 0.299*R + 0.587*G + 0.114*B
                uint8_t gray = static_cast<uint8_t>(static_cast<int>(0.299 * r + 0.587 * g + 0.114 * b));
                grayData[y * width + x] = gray;
            }
        }

        auto clone = Image(width, height, 1, std::move(grayData));

        clone.origin = origin;

        return clone;
    }


    Image Image::load_from_memory(const uint8_t *buffer, const size_t size) {
        int w = 0, h = 0, c = 0;
        stbi_uc *pixels = stbi_load_from_memory(buffer, static_cast<int>(size), &w, &h, &c, 0);

        if (!pixels) {
            throw std::runtime_error("[Image::load_from_memory] Failed to decode image from memory");
        }

        std::vector<uint8_t> data(pixels, pixels + (w * h * c));
        stbi_image_free(pixels);

        auto clone = Image(w, h, c, std::move(data));

        clone.origin = "memory";

        return clone;
    }

    Image Image::load_from_memory(const std::vector<uint8_t> &buffer) {
        return load_from_memory(buffer.data(), buffer.size());
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
        // Validate coordinates
        if (x < 0 || x >= width || y < 0 || y >= height) {
            throw std::out_of_range("[Image::getRGB] Coordinates out of bounds");
        }

        if (!isValid()) {
            throw std::runtime_error("[Image::getRGB] Invalid image");
        }

        // Calculate the index in the data array
        size_t index = (static_cast<size_t>(y) * width + x) * channels;

        std::array<uint8_t, 3> rgb;

        if (channels == 1) {
            // Grayscale: return same value for R, G, B
            uint8_t gray = data[index];
            rgb = {gray, gray, gray};
        } else if (channels == 3) {
            // RGB: return R, G, B
            rgb = {data[index], data[index + 1], data[index + 2]};
        } else if (channels == 4) {
            // RGBA: return R, G, B (ignore alpha)
            rgb = {data[index], data[index + 1], data[index + 2]};
        } else {
            throw std::runtime_error("[Image::getRGB] Unsupported channel count");
        }

        return rgb;
    }
}
