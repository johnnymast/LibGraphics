#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <filesystem>
#include <chrono>
#include <sstream>

namespace LibGraphics {

    struct Region {
        int x;
        int y;
        int w;
        int h;
    };

    struct Image {
        std::vector<uint8_t> data;
        int width = 0;
        int height = 0;
        int channels = 0;
        std::string origin = "empty";

        Image() = default;
        Image(int width, int height, int channels, std::vector<uint8_t> pixels);

        static Image load(const std::string &path);
        static Image load_from_memory(const uint8_t* buffer, size_t size);
        static Image load_from_memory(const std::vector<uint8_t>& buffer);

        [[nodiscard]] bool save(const std::string &path, int quality = 90) const;
        void show() const;

        Image toGrayscale() const;

        [[nodiscard]] std::array<uint8_t, 3> getRGB(int x, int y) const;
        [[nodiscard]] Image crop(int x, int y, int width, int height) const;
        [[nodiscard]] bool isValid() const;
        [[nodiscard]] Image clone() const;

        void redact(const Region& region, uint8_t value = 0);
        void redact(const std::vector<Region>& regions, uint8_t value = 0);



    private:
        static std::string mkTempFilename(const std::string& prefix = "libgraphics_", const std::string& ext = ".png") {
            auto now = std::chrono::system_clock::now().time_since_epoch().count();
            std::ostringstream oss;
            oss << prefix << now << ext;
            return std::filesystem::temp_directory_path() / oss.str();
        }
    };
}
