#include "LibGraphics/LibGraphics.hpp"

#include <opencv2/core/version.hpp>

#ifdef OPENCV_WITH_CUDA
#include <opencv2/core/cuda.hpp>
#endif

namespace LibGraphics {
    OpenCVInfo GetOpenCVInfo() {

        bool haveCuda = false;

#if defined(OPENCV_WITH_CUDA)
        if (cv::cuda::getCudaEnabledDeviceCount() > 0) {
            haveCuda = true;
        }
#endif

        return OpenCVInfo{CV_VERSION_MAJOR, CV_VERSION_MINOR, CV_VERSION_REVISION, haveCuda};
    }
}
