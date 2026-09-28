#include "AsciiConverter.hpp"
#include <opencv2/imgproc.hpp>
#include <algorithm>

AsciiConverter::AsciiConverter(int asciiWidth, const std::string& charset)
    : asciiWidth_(asciiWidth), charset_(charset) {}

std::string AsciiConverter::convert(const cv::Mat& frame) {
    if (frame.empty()) {
        return "";
    }

    cv::Mat processed = preprocess(frame);
    if (processed.empty()) {
        return "";
    }

    int asciiHeight = static_cast<int>(processed.rows * aspectCorrection_ *
        static_cast<float>(asciiWidth_) / processed.cols);
    if (asciiHeight <= 0) asciiHeight = 1;

    std::string result;
    result.reserve(static_cast<size_t>(asciiHeight) * (asciiWidth_ + 1));

    for (int y = 0; y < asciiHeight; ++y) {
        for (int x = 0; x < asciiWidth_; ++x) {
            int srcY = static_cast<int>(static_cast<float>(y) * processed.rows / asciiHeight);
            int srcX = static_cast<int>(static_cast<float>(x) * processed.cols / asciiWidth_);

            srcY = std::min(srcY, processed.rows - 1);
            srcX = std::min(srcX, processed.cols - 1);

            float brightness = static_cast<float>(processed.at<uchar>(srcY, srcX)) / 255.0f;
            result.push_back(mapBrightness(brightness));
        }
        result.push_back('\n');
    }

    return result;
}

void AsciiConverter::setAsciiWidth(int width) {
    asciiWidth_ = std::max(1, width);
}

void AsciiConverter::setCharset(const std::string& charset) {
    if (!charset.empty()) {
        charset_ = charset;
    }
}

void AsciiConverter::setInvert(bool invert) {
    invert_ = invert;
}

void AsciiConverter::setAspectCorrection(float correction) {
    aspectCorrection_ = std::max(0.1f, correction);
}

cv::Mat AsciiConverter::preprocess(const cv::Mat& frame) const {
    cv::Mat gray;
    if (frame.channels() == 3) {
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
    } else if (frame.channels() == 4) {
        cv::cvtColor(frame, gray, cv::COLOR_BGRA2GRAY);
    } else {
        gray = frame.clone();
    }

    return gray;
}

char AsciiConverter::mapBrightness(float brightness) const {
    if (invert_) {
        brightness = 1.0f - brightness;
    }

    brightness = std::clamp(brightness, 0.0f, 1.0f);

    size_t index = static_cast<size_t>(brightness * (charset_.size() - 1));
    index = std::min(index, charset_.size() - 1);

    return charset_[index];
}