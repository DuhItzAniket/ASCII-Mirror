#include "FrameProcessor.hpp"
#include <opencv2/imgproc.hpp>

cv::Mat FrameProcessor::process(const cv::Mat& frame) {
    if (frame.empty()) {
        return cv::Mat();
    }

    switch (currentFilter_) {
        case FilterType::Grayscale:
            return applyGrayscale(frame);
        case FilterType::Invert:
            return applyInvert(frame);
        case FilterType::Threshold:
            return applyThreshold(frame);
        case FilterType::Edge:
            return applyEdge(frame);
        case FilterType::Blur:
            return applyBlur(frame);
        case FilterType::None:
        default:
            return frame.clone();
    }
}

void FrameProcessor::setFilter(FilterType filter) {
    currentFilter_ = filter;
}

void FrameProcessor::setThresholdValue(double value) {
    thresholdValue_ = std::clamp(value, 0.0, 255.0);
}

void FrameProcessor::setBlurKernelSize(int size) {
    blurKernelSize_ = (size > 0 && size % 2 == 1) ? size : 3;
}

cv::Mat FrameProcessor::applyGrayscale(const cv::Mat& frame) const {
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

cv::Mat FrameProcessor::applyInvert(const cv::Mat& frame) const {
    cv::Mat gray = applyGrayscale(frame);
    cv::bitwise_not(gray, gray);
    return gray;
}

cv::Mat FrameProcessor::applyThreshold(const cv::Mat& frame) const {
    cv::Mat gray = applyGrayscale(frame);
    cv::threshold(gray, gray, thresholdValue_, 255, cv::THRESH_BINARY);
    return gray;
}

cv::Mat FrameProcessor::applyEdge(const cv::Mat& frame) const {
    cv::Mat gray = applyGrayscale(frame);
    cv::Mat edges;
    cv::Canny(gray, edges, 50, 150);
    cv::bitwise_not(edges, edges);
    return edges;
}

cv::Mat FrameProcessor::applyBlur(const cv::Mat& frame) const {
    cv::Mat gray = applyGrayscale(frame);
    cv::GaussianBlur(gray, gray, cv::Size(blurKernelSize_, blurKernelSize_), 0);
    return gray;
}