#pragma once

#include "LibGraphics/Image.hpp"

#include <leptonica/allheaders.h>
#include <opencv2/core/mat.hpp>

using LibGraphics::Image;

namespace LibGraphics::Utils {
    class Converter {
    public:
        static cv::Mat ImageToMat(const Image &image);
        static Image MatToImage(const cv::Mat &mat);
        static Pix *imageToPix(Image &image);
        static Image pixToImage(Pix *pix);
    };
};