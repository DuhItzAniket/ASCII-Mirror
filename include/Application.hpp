#pragma once

#include "Camera.hpp"
#include "AsciiConverter.hpp"
#include "TerminalRenderer.hpp"
#include "FrameProcessor.hpp"
#include "Config.hpp"

class Application {
public:
    explicit Application(const Config& config);
    ~Application();

    int run();

private:
    void processInput();
    void updateFps();
    void printHelp() const;
    void printStats() const;

    Config config_;
    Camera camera_;
    AsciiConverter converter_;
    TerminalRenderer renderer_;
    FrameProcessor processor_;

    bool running_ = false;
    int frameCount_ = 0;
    double fps_ = 0.0;
    std::chrono::steady_clock::time_point lastFpsUpdate_;
    mutable cv::Mat frameBuffer_;
};