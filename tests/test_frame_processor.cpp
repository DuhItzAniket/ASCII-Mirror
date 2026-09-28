#include "FrameProcessor.hpp"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <cassert>
#include <iostream>

void test_grayscale_filter() {
    cv::Mat bgr(100, 100, CV_8UC3, cv::Scalar(255, 0, 0));
    FrameProcessor processor;
    processor.setFilter(FilterType::Grayscale);
    cv::Mat result = processor.process(bgr);

    assert(result.channels() == 1);
    assert(result.rows == 100 && result.cols == 100);
    std::cout << "test_grayscale_filter: PASSED\n";
}

void test_invert_filter() {
    cv::Mat gray(100, 100, CV_8UC1, cv::Scalar(100));
    FrameProcessor processor;
    processor.setFilter(FilterType::Invert);
    cv::Mat result = processor.process(gray);

    assert(result.channels() == 1);
    assert(result.at<uchar>(0, 0) == 155);
    std::cout << "test_invert_filter: PASSED\n";
}

void test_threshold_filter() {
    cv::Mat gray(100, 100, CV_8UC1, cv::Scalar(100));
    FrameProcessor processor;
    processor.setFilter(FilterType::Threshold);
    processor.setThresholdValue(128);
    cv::Mat result = processor.process(gray);

    assert(result.channels() == 1);
    assert(result.at<uchar>(0, 0) == 0);
    std::cout << "test_threshold_filter: PASSED\n";
}

void test_edge_filter() {
    cv::Mat gray(100, 100, CV_8UC1, cv::Scalar(128));
    cv::rectangle(gray, cv::Rect(20, 20, 60, 60), cv::Scalar(255), -1);
    FrameProcessor processor;
    processor.setFilter(FilterType::Edge);
    cv::Mat result = processor.process(gray);

    assert(result.channels() == 1);
    assert(result.rows == 100 && result.cols == 100);
    std::cout << "test_edge_filter: PASSED\n";
}

void test_blur_filter() {
    cv::Mat gray(100, 100, CV_8UC1);
    cv::randu(gray, 0, 256);
    FrameProcessor processor;
    processor.setFilter(FilterType::Blur);
    processor.setBlurKernelSize(5);
    cv::Mat result = processor.process(gray);

    assert(result.channels() == 1);
    assert(result.rows == 100 && result.cols == 100);
    std::cout << "test_blur_filter: PASSED\n";
}

void test_none_filter() {
    cv::Mat bgr(100, 100, CV_8UC3, cv::Scalar(255, 0, 0));
    FrameProcessor processor;
    processor.setFilter(FilterType::None);
    cv::Mat result = processor.process(bgr);

    assert(result.channels() == 3);
    assert(result.rows == 100 && result.cols == 100);
    std::cout << "test_none_filter: PASSED\n";
}

void test_empty_frame() {
    cv::Mat empty;
    FrameProcessor processor;
    cv::Mat result = processor.process(empty);

    assert(result.empty());
    std::cout << "test_empty_frame: PASSED\n";
}

int main() {
    std::cout << "Running FrameProcessor tests...\n\n";

    test_grayscale_filter();
    test_invert_filter();
    test_threshold_filter();
    test_edge_filter();
    test_blur_filter();
    test_none_filter();
    test_empty_frame();

    std::cout << "\nAll tests passed!\n";
    return 0;
}