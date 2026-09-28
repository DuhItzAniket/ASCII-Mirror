#include "AsciiConverter.hpp"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <cassert>
#include <iostream>

void test_black_image() {
    cv::Mat black(100, 100, CV_8UC1, cv::Scalar(0));
    AsciiConverter converter(20, "@%#*+=-:. ");
    std::string result = converter.convert(black);

    assert(!result.empty());
    for (char c : result) {
        if (c != '\n') assert(c == '@');
    }
    std::cout << "test_black_image: PASSED\n";
}

void test_white_image() {
    cv::Mat white(100, 100, CV_8UC1, cv::Scalar(255));
    AsciiConverter converter(20, "@%#*+=-:. ");
    std::string result = converter.convert(white);

    assert(!result.empty());
    for (char c : result) {
        if (c != '\n') assert(c == ' ');
    }
    std::cout << "test_white_image: PASSED\n";
}

void test_mid_gray() {
    cv::Mat gray(100, 100, CV_8UC1, cv::Scalar(128));
    AsciiConverter converter(20, "@%#*+=-:. ");
    std::string result = converter.convert(gray);

    assert(!result.empty());
    std::cout << "test_mid_gray: PASSED\n";
}

void test_gradient() {
    // Image size 100x100, asciiWidth=20, aspectCorrection=1.0 -> asciiHeight = 100 * 1.0 * 20/100 = 20
    // Let's use 20x20 image with asciiWidth=20, aspectCorrection=1.0 -> asciiHeight = 20
    cv::Mat gradient(20, 20, CV_8UC1);
    for (int y = 0; y < 20; ++y) {
        for (int x = 0; x < 20; ++x) {
            gradient.at<uchar>(y, x) = static_cast<uchar>(x * 255 / 20);
        }
    }
    AsciiConverter converter(20, "@%#*+=-:. ");
    converter.setAspectCorrection(1.0f);
    std::string result = converter.convert(gradient);

    assert(!result.empty());
    size_t lines = 0;
    for (char c : result) if (c == '\n') lines++;
    assert(lines == 20);
    std::cout << "test_gradient: PASSED\n";
}

void test_invert() {
    cv::Mat black(100, 100, CV_8UC1, cv::Scalar(0));
    AsciiConverter converter(20, "@%#*+=-:. ");
    converter.setInvert(true);
    std::string result = converter.convert(black);

    assert(!result.empty());
    for (char c : result) {
        if (c != '\n') assert(c == ' ');
    }
    std::cout << "test_invert: PASSED\n";
}

void test_bgr_input() {
    cv::Mat bgr(100, 100, CV_8UC3, cv::Scalar(0, 0, 255));
    AsciiConverter converter(20, "@%#*+=-:. ");
    std::string result = converter.convert(bgr);

    assert(!result.empty());
    std::cout << "test_bgr_input: PASSED\n";
}

void test_empty_image() {
    cv::Mat empty;
    AsciiConverter converter(20, "@%#*+=-:. ");
    std::string result = converter.convert(empty);

    assert(result.empty());
    std::cout << "test_empty_image: PASSED\n";
}

void test_ascii_width() {
    cv::Mat gray(100, 100, CV_8UC1, cv::Scalar(128));
    AsciiConverter converter(40, "@%#*+=-:. ");
    std::string result = converter.convert(gray);

    size_t lineLength = result.find('\n');
    assert(lineLength == 40);
    std::cout << "test_ascii_width: PASSED\n";
}

int main() {
    std::cout << "Running AsciiConverter tests...\n\n";

    test_black_image();
    test_white_image();
    test_mid_gray();
    test_gradient();
    test_invert();
    test_bgr_input();
    test_empty_image();
    test_ascii_width();

    std::cout << "\nAll tests passed!\n";
    return 0;
}