#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <filesystem>
#include <chrono>
#include <sstream>

namespace LibGraphics {

    struct Image {
        std::vector<uint8_t> data;
        int width = 0;
        int height = 0;
        int channels = 0;

        static Image load(const std::string &path);
        bool save(const std::string &path, int quality = 90) const;
        void show() const;

        Image crop(int x, int y, int width, int height) const;
        Image clone() const;


    private:
        static std::string mkTempFilename(const std::string& prefix = "libgraphics_", const std::string& ext = ".png") {
            auto now = std::chrono::system_clock::now().time_since_epoch().count();
            std::ostringstream oss;
            oss << prefix << now << ext;
            return std::filesystem::temp_directory_path() / oss.str();
        }
    };
}
