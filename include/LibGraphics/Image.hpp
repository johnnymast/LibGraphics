#pragma once

#include "modules/stb_image_write.h"
#include "modules/stb_image.h"

#include <cstdint>
#include <string>
#include <vector>
#include <opencv2/core/mat.hpp>

namespace LibGraphics {
    struct Image {
        std::vector<uint8_t> data;  // Raw pixel data (e.g., RGB or RGBA)
        int width = 0;
        int height = 0;
        int channels = 0;

        static Image load(const std::string& path);

        bool save(const std::string& path, int quality = 90) const;

        [[nodiscard]] [[deprecated("Use the converter class instead")]]
        static Image fromMat(const cv::Mat& mat);

        [[nodiscard]] [[deprecated("Use member data instead")]]
        const uint8_t* getData() const { return data.data(); }

        [[nodiscard]] [[deprecated("Use member data instead")]]
        uint8_t* getData() { return data.data(); }

        [[nodiscard]] [[deprecated("Use member width instead")]]
        int getWidth() const { return width; }

        [[nodiscard]] [[deprecated("Use member height instead")]]
        int getHeight() const { return height; }

        [[nodiscard]] [[deprecated("Use member channels instead")]]
        int getChannels() const { return channels; }
   };
}