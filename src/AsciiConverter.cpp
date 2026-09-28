#include "AsciiConverter.hpp"
#include <opencv2/imgproc.hpp>
#include <algorithm>
#include <chrono>

AsciiConverter::AsciiConverter(int asciiWidth, const std::string& charset)
    : asciiWidth_(asciiWidth), charset_(charset) {
    rebuildLut();
}

std::string AsciiConverter::convert(const cv::Mat& frame) {
    if (frame.empty()) {
        return "";
    }

    auto start = std::chrono::high_resolution_clock::now();
    cv::Mat processed = preprocess(frame);
    auto preprocessEnd = std::chrono::high_resolution_clock::now();

    if (processed.empty()) {
        return "";
    }

    if (lutDirty_) {
        rebuildLut();
    }

    int asciiHeight = static_cast<int>(processed.rows * aspectCorrection_ *
        static_cast<float>(asciiWidth_) / processed.cols);
    if (asciiHeight <= 0) asciiHeight = 1;

    std::string result;
    result.reserve(static_cast<size_t>(asciiHeight) * (asciiWidth_ + 1));

    const float rowScale = static_cast<float>(processed.rows) / asciiHeight;
    const float colScale = static_cast<float>(processed.cols) / asciiWidth_;

    for (int y = 0; y < asciiHeight; ++y) {
        const int srcY = std::min(static_cast<int>(y * rowScale), processed.rows - 1);
        const uchar* rowPtr = processed.ptr<uchar>(srcY);

        for (int x = 0; x < asciiWidth_; ++x) {
            const int srcX = std::min(static_cast<int>(x * colScale), processed.cols - 1);
            result.push_back(lut_[rowPtr[srcX]]);
        }
        result.push_back('\n');
    }

    auto end = std::chrono::high_resolution_clock::now();
    stats_.convertTime += std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    stats_.preprocessTime += std::chrono::duration_cast<std::chrono::microseconds>(preprocessEnd - start);
    stats_.framesProcessed++;

    return result;
}

void AsciiConverter::setAsciiWidth(int width) {
    asciiWidth_ = std::max(1, width);
}

void AsciiConverter::setCharset(const std::string& charset) {
    if (!charset.empty()) {
        charset_ = charset;
        lutDirty_ = true;
    }
}

void AsciiConverter::setInvert(bool invert) {
    if (invert_ != invert) {
        invert_ = invert;
        lutDirty_ = true;
    }
}

void AsciiConverter::setCharset(PresetCharset preset) {
    setCharset(presetToString(preset));
}

void AsciiConverter::setAspectCorrection(float correction) {
    aspectCorrection_ = std::max(0.1f, correction);
}

std::string AsciiConverter::presetToString(PresetCharset preset) {
    switch (preset) {
        case PresetCharset::Standard:
            return "@%#*+=-:. ";
        case PresetCharset::Dense:
            return "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`'. ";
        case PresetCharset::Blocks:
            return "█▓▒░ ";
        case PresetCharset::Minimal:
            return "@# ";
    }
    return "@%#*+=-:. ";
}

cv::Mat AsciiConverter::preprocess(const cv::Mat& frame) const {
    if (frame.channels() == 1) {
        return frame;
    }

    if (grayBuffer_.size() != frame.size() || grayBuffer_.type() != CV_8UC1) {
        grayBuffer_.create(frame.size(), CV_8UC1);
    }

    if (frame.channels() == 3) {
        cv::cvtColor(frame, grayBuffer_, cv::COLOR_BGR2GRAY);
    } else if (frame.channels() == 4) {
        cv::cvtColor(frame, grayBuffer_, cv::COLOR_BGRA2GRAY);
    }

    return grayBuffer_;
}

void AsciiConverter::rebuildLut() const {
    for (int i = 0; i < 256; ++i) {
        float brightness = static_cast<float>(i) / 255.0f;
        if (invert_) brightness = 1.0f - brightness;
        brightness = std::clamp(brightness, 0.0f, 1.0f);
        size_t index = static_cast<size_t>(brightness * (charset_.size() - 1));
        index = std::min(index, charset_.size() - 1);
        lut_[i] = charset_[index];
    }
    lutDirty_ = false;
}

char AsciiConverter::mapBrightness(float brightness) const {
    if (invert_) brightness = 1.0f - brightness;
    brightness = std::clamp(brightness, 0.0f, 1.0f);
    size_t index = static_cast<size_t>(brightness * (charset_.size() - 1));
    index = std::min(index, charset_.size() - 1);
    return charset_[index];
}