#include "opencv_info.hpp"
#include "../include/LibGraphics/export.hpp"
#include <opencv2/core/version.hpp>

LIBGRAPHICS_API std::string getOpenCVInfo() {
    return std::string("OpenCV version: ") + CV_VERSION;
}
