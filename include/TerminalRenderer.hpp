#pragma once

#include <opencv2/core.hpp>
#include <string>

class TerminalRenderer {
public:
    enum class ColorMode {
        None,
        Grayscale,
        TrueColor
    };

    TerminalRenderer();
    ~TerminalRenderer();

    void initialize();
    void render(const std::string& frame);
    void renderColor(const cv::Mat& frame, int asciiWidth, float aspectCorrection);
    void shutdown();
    void clear();
    void moveCursorHome();

    int getTerminalWidth() const;
    int getTerminalHeight() const;

    bool isInitialized() const { return initialized_; }
    void setColorMode(ColorMode mode) { colorMode_ = mode; }
    ColorMode getColorMode() const { return colorMode_; }

private:
    bool initialized_ = false;
    int terminalWidth_ = 80;
    int terminalHeight_ = 24;
    std::string lastFrame_;
    ColorMode colorMode_ = ColorMode::None;

    void getTerminalSize();
    void enableAnsiSupport();
    std::string pixelToAnsiColor(int r, int g, int b) const;
};