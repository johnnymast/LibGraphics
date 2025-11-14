#include "LibGraphics.hpp"

#include <iostream>
#include <opencv2/core/cuda.hpp>

namespace LibGraphics {
    OpenCVInfo GetOpenCVInfo() {

        bool haveCuda = false;
        int deviceCount = cv::cuda::getCudaEnabledDeviceCount();
        if (deviceCount > 0) {

            for (int i = 0; i < deviceCount; ++i) {
                cv::cuda::DeviceInfo devInfo(i);
                std::cout << "Device " << i << ": " << devInfo.name()
                          << " (Compute capability " << devInfo.majorVersion()
                          << "." << devInfo.minorVersion() << ")" << std::endl;
            }
        } else {
            std::cout << "No CUDA devices found or OpenCV not built with CUDA." << std::endl;
        }

        return OpenCVInfo{CV_VERSION_MAJOR, CV_VERSION_MINOR, CV_VERSION_REVISION, haveCuda};
    }
}
