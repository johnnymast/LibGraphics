#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#include "LibGraphics/Image.hpp"
#include "modules/stb_image_write.h"
#include "modules/stb_image.h"

#include <opencv2/opencv.hpp>
#include <stdexcept>

namespace LibGraphics {
    Image Image::load(const std::string& path) {
        Image img;

        int width, height, channels;
        unsigned char* imageData = stbi_load(path.c_str(), &width, &height, &channels, 0);

        if (!imageData) {
            throw std::runtime_error("Failed to load image: " + path);
        }

        img.width = width;
        img.height = height;
        img.channels = channels;

        size_t dataSize = static_cast<size_t>(width) * height * channels;
        img.data.assign(imageData, imageData + dataSize);

        stbi_image_free(imageData);

        return img;
    }

    bool Image::save(const std::string& path, int quality) const {
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

    Image Image::crop(int x, int y, int width, int height) const {
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

    Image Image::clone() const {
        Image copy = *this;
        return copy;
    }
}