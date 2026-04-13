#pragma once

#include "LibGraphics/type/Rect.hpp"

#include <cstdint>
#include <string>
#include <vector>
#include <filesystem>
#include <array>


#include <opencv2/core.hpp>

#include "export.hpp"

using LibGraphics::Type::Rect;

namespace LibGraphics {

    struct LIBGRAPHICS_API Image {
        std::vector<uint8_t> data;
        int width = 0;
        int height = 0;
        int channels = 0;
        std::string origin = "empty";

        Image() = default;
        Image(int width, int height, int channels, std::vector<uint8_t> pixels);

        static Image load(const std::string& path);
        static Image load_from_memory(const uint8_t* buffer, size_t size);
        static Image load_from_memory(const std::vector<uint8_t>& buffer);

        [[nodiscard]] bool save(const std::string& path, int quality = 90) const;
        void show() const;

        [[nodiscard]] Image toGrayscale() const;
        [[nodiscard]] Image crop(int x, int y, int width, int height) const;
        [[nodiscard]] Image clone() const;

        [[nodiscard]] std::array<uint8_t, 3> getRGB(int x, int y) const;
        [[nodiscard]] bool isValid() const;

        explicit operator bool() const { return isValid(); }

        void redact(const Rect& roi, uint8_t value = 0);
        void redact(const std::vector<Rect>& rois, uint8_t value = 0);

        cv::Mat& mat();             // kleur
        const cv::Mat& mat() const;

        cv::Mat& matGray();         // zwart-wit
        const cv::Mat& matGray() const;

    private:
        mutable cv::Mat cachedColor;
        mutable cv::Mat cachedGray;

        void invalidateCache();
        static void stripAlpha(std::vector<uint8_t>& pixels, int width, int height, int& channels);

        static std::string mkTempFilename(
            const std::string& prefix = "libgraphics_",
            const std::string& ext = ".png"
        );
    };

}
