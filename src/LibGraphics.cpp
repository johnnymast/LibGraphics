#include "LibGraphics.hpp"

#include <iostream>
#include <opencv2/core/cuda.hpp>

namespace LibGraphics {
    OpenCVInfo GetOpenCVInfo() {

        bool haveCuda = false;
        if (int deviceCount = cv::cuda::getCudaEnabledDeviceCount(); deviceCount > 0) {
            haveCuda = true;
        }

        return OpenCVInfo{CV_VERSION_MAJOR, CV_VERSION_MINOR, CV_VERSION_REVISION, haveCuda};
    }
}
