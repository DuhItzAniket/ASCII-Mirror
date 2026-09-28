#pragma once

#include <opencv2/core.hpp>
#include <string>
#include <vector>
#include <chrono>
#include <array>

class AsciiConverter {
public:
    enum class PresetCharset {
        Standard,     // @%#*+=-:. 
        Dense,        // $@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\|()1{}[]?-_+~<>i!lI;:,"^`'.
        Blocks,       // █▓▒░ 
        Minimal       // @# 
    };

    explicit AsciiConverter(int asciiWidth = 120, const std::string& charset = "@%#*+=-:. ");

    std::string convert(const cv::Mat& frame);
    void setAsciiWidth(int width);
    void setCharset(const std::string& charset);
    void setCharset(PresetCharset preset);
    void setInvert(bool invert);
    void setAspectCorrection(float correction);

    int getAsciiWidth() const { return asciiWidth_; }
    const std::string& getCharset() const { return charset_; }
    bool getInvert() const { return invert_; }

    struct Stats {
        std::chrono::microseconds convertTime{0};
        std::chrono::microseconds preprocessTime{0};
        int framesProcessed = 0;
    };
    Stats getStats() const { return stats_; }
    void resetStats() { stats_ = {}; }

private:
    int asciiWidth_ = 120;
    std::string charset_ = "@%#*+=-:. ";
    bool invert_ = false;
    float aspectCorrection_ = 0.5f;

    mutable cv::Mat grayBuffer_;
    mutable std::array<char, 256> lut_{};
    mutable bool lutDirty_ = true;

    Stats stats_;

    cv::Mat preprocess(const cv::Mat& frame) const;
    void rebuildLut() const;
    char mapBrightness(float brightness) const;
    static std::string presetToString(PresetCharset preset);
};