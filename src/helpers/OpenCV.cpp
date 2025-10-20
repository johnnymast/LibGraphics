#include "LibGraphics/helpers/OpenCV.hpp"

#include <stdexcept>

namespace LibGraphics::Helpers {
    cv::Mat OpenCv::ImageToMat(const Image& image) {

        if (image.data.empty() || image.width <= 0 || image.height <= 0 || image.channels <= 0) {
            return {};
        }

        int cvType;
        switch (image.channels) {
            case 1:
                cvType = CV_8UC1;
                break;
            case 3:
                cvType = CV_8UC3;
                break;
            case 4:
                cvType = CV_8UC4;
                break;
            default:
                throw std::runtime_error("Unsupported number of channels: " + std::to_string(image.channels));
        }

        cv::Mat mat(image.height, image.width, cvType, const_cast<uint8_t*>(image.data.data()));

        return mat.clone();
    }

    Image OpenCv::MatToImage(const cv::Mat &mat) {
        Image img;

        if (mat.empty()) {
            throw std::runtime_error("Cannot convert empty cv::Mat to Image");
        }

        img.width = mat.cols;
        img.height = mat.rows;
        img.channels = mat.channels();

        // Convert Mat to continuous if necessary
        cv::Mat continuous = mat.isContinuous() ? mat : mat.clone();

        // Copy data
        size_t dataSize = static_cast<size_t>(img.width) * img.height * img.channels;
        img.data.assign(continuous.data, continuous.data + dataSize);

        return img;
    }

}
