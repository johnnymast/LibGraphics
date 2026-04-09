#include "LibGraphics/LibGraphics.hpp"

#include <opencv2/core/version.hpp>

namespace LibGraphics {
    OpenCVInfo GetOpenCVInfo() {

        bool haveCuda = false;

#if defined(HAVE_OPENCV_CUDAARITHM)
        if (cv::cuda::getCudaEnabledDeviceCount() > 0) {
            haveCuda = true;
        }
#endif

        return OpenCVInfo{CV_VERSION_MAJOR, CV_VERSION_MINOR, CV_VERSION_REVISION, haveCuda};
    }
}
