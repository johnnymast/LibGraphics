#pragma once

#include "modules/stb_image_write.h"
#include "modules/stb_image.h"

#include <cstdint>
#include <string>
#include <vector>
#include <opencv2/core/mat.hpp>

struct Image {
    std::vector<uint8_t> data;  // Raw pixel data (e.g., RGB or RGBA)
    int width = 0;
    int height = 0;
    int channels = 0;

    // Load from disk using stb_image
    static Image load(const std::string& path);

    // Save to disk using stb_image_write
    bool save(const std::string& path, int quality = 90) const;

    static Image fromMat(const cv::Mat& mat);

    // Accessor methods for image properties
    const uint8_t* getData() const { return data.data(); }
    uint8_t* getData() { return data.data(); }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getChannels() const { return channels; }

    // friend std::ostream& operator<<(std::ostream& os, const Image& image);

};