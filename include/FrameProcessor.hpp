#pragma once

#include <opencv2/core.hpp>

enum class FilterType {
    None,
    Grayscale,
    Invert,
    Threshold,
    Edge,
    Blur
};

class FrameProcessor {
public:
    FrameProcessor() = default;

    cv::Mat process(const cv::Mat& frame);
    void setFilter(FilterType filter);
    void setThresholdValue(double value);
    void setBlurKernelSize(int size);
    FilterType getFilter() const { return currentFilter_; }

private:
    FilterType currentFilter_ = FilterType::Grayscale;
    double thresholdValue_ = 128.0;
    int blurKernelSize_ = 3;

    cv::Mat applyGrayscale(const cv::Mat& frame) const;
    cv::Mat applyInvert(const cv::Mat& frame) const;
    cv::Mat applyThreshold(const cv::Mat& frame) const;
    cv::Mat applyEdge(const cv::Mat& frame) const;
    cv::Mat applyBlur(const cv::Mat& frame) const;
};