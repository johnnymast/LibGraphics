#pragma once

#include "../Image.hpp"

namespace LibGraphics::Helpers {
    class OpenCv {
    public:
        static cv::Mat ImageToMat(const Image& image);
        static Image MatToImage(const cv::Mat& mat);
    };
};
