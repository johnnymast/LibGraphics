#pragma once

#include "LibGraphics/type/Rect.hpp"

#include <cstdint>
#include <string>
#include <vector>
#include <filesystem>
#include <chrono>
#include <array>
#include <sstream>

#include "export.hpp"

using LibGraphics::Type::Rect;

namespace LibGraphics {
    struct LIBGRAPHICS_API Image {
        std::vector<uint8_t> data;
        int width = 0;
        int height = 0;
        int channels = 0; // 1 = grayscale, 3 = RGB
        std::string origin = "empty";

        // Image() = default;
        Image() = default;

        // Enige constructor die we implementeren in Image.cpp
        Image(int width, int height, int channels, std::vector<uint8_t> pixels);

        // Loading
        static Image load(const std::string &path);
        static Image load_from_memory(const uint8_t *buffer, size_t size);
        static Image load_from_memory(const std::vector<uint8_t> &buffer);

        // Saving + preview
        [[nodiscard]] bool save(const std::string &path, int quality = 90) const;

        void show() const;

        // Transformations
        [[nodiscard]] Image toGrayscale() const;

        [[nodiscard]] Image crop(int x, int y, int width, int height) const;

        [[nodiscard]] Image clone() const;

        // Info
        [[nodiscard]] std::array<uint8_t, 3> getRGB(int x, int y) const;

        [[nodiscard]] bool isValid() const;

        // Allows: if (image)
        explicit operator bool() const {
            return isValid();
        }

        // Redaction
        void redact(const Type::Rect &roi, uint8_t value = 0);

        void redact(const std::vector<Type::Rect> &rois, uint8_t value = 0);

    private:
        static std::string mkTempFilename(
            const std::string &prefix = "libgraphics_",
            const std::string &ext = ".png"
        ) {
            auto now = std::chrono::system_clock::now().time_since_epoch().count();
            std::ostringstream oss;
            oss << prefix << now << ext;
            return (std::filesystem::temp_directory_path() / oss.str()).string();
        }

        static void stripAlpha(std::vector<uint8_t> &pixels, int width, int height, int &channels);
    };
}
