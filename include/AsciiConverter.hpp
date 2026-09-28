#pragma once

#include <opencv2/core.hpp>
#include <string>
#include <vector>

class AsciiConverter {
public:
    explicit AsciiConverter(int asciiWidth = 120, const std::string& charset = "@%#*+=-:. ");

    std::string convert(const cv::Mat& frame);
    void setAsciiWidth(int width);
    void setCharset(const std::string& charset);
    void setInvert(bool invert);
    void setAspectCorrection(float correction);

    int getAsciiWidth() const { return asciiWidth_; }
    const std::string& getCharset() const { return charset_; }
    bool getInvert() const { return invert_; }

private:
    int asciiWidth_ = 120;
    std::string charset_ = "@%#*+=-:. ";
    bool invert_ = false;
    float aspectCorrection_ = 0.5f;

    cv::Mat preprocess(const cv::Mat& frame) const;
    char mapBrightness(float brightness) const;
};