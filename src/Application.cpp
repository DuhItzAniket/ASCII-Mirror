#include "Application.hpp"
#include <iostream>
#include <chrono>
#include <thread>
#include <conio.h>

Application::Application(const Config& config)
    : config_(config),
      converter_(config.asciiWidth, config.charset),
      lastFpsUpdate_(std::chrono::steady_clock::now()) {
    converter_.setInvert(config.invert);
    converter_.setAspectCorrection(config.aspectCorrection);

    if (config.filterMode == "invert") processor_.setFilter(FilterType::Invert);
    else if (config.filterMode == "threshold") processor_.setFilter(FilterType::Threshold);
    else if (config.filterMode == "edge") processor_.setFilter(FilterType::Edge);
    else if (config.filterMode == "blur") processor_.setFilter(FilterType::Blur);
    else processor_.setFilter(FilterType::Grayscale);
}

Application::~Application() {
    camera_.release();
    renderer_.shutdown();
}

int Application::run() {
    std::cout << "========================================\n";
    std::cout << "       ASCII MIRROR ENGINE\n";
    std::cout << "========================================\n\n";

    std::cout << "Camera: " << config_.cameraIndex << "\n";
    std::cout << "ASCII Width: " << config_.asciiWidth << "\n";
    std::cout << "Filter: " << config_.filterMode << "\n";
    std::cout << "Invert: " << (config_.invert ? "On" : "Off") << "\n\n";

    std::cout << "Controls:\n";
    std::cout << "  Q, ESC    Quit\n";
    std::cout << "  +, =      Increase ASCII width\n";
    std::cout << "  -         Decrease ASCII width\n";
    std::cout << "  1-5       Change filter mode\n";
    std::cout << "  0         No filter\n\n";

    std::cout << "Starting camera...\n";

    if (!camera_.open(config_.cameraIndex)) {
        std::cerr << "Failed to open camera. Exiting.\n";
        return 1;
    }

    std::cout << "Camera opened: " << camera_.getWidth() << "x" << camera_.getHeight()
              << " @ " << camera_.getFPS() << " FPS\n\n";

    renderer_.initialize();

    running_ = true;
    cv::Mat frame;
    auto frameStart = std::chrono::steady_clock::now();
    const auto frameDuration = std::chrono::milliseconds(1000 / config_.targetFps);

    while (running_) {
        frameStart = std::chrono::steady_clock::now();

        if (!camera_.read(frame) || frame.empty()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }

        cv::Mat processed = processor_.process(frame);
        std::string ascii = converter_.convert(processed);

        if (config_.showFps) {
            ascii += "\nFPS: " + std::to_string(static_cast<int>(fps_));
        }

        renderer_.render(ascii);
        processInput();
        updateFps();

        auto elapsed = std::chrono::steady_clock::now() - frameStart;
        if (elapsed < frameDuration) {
            std::this_thread::sleep_for(frameDuration - elapsed);
        }
    }

    renderer_.shutdown();
    std::cout << "\nShutdown complete.\n";
    return 0;
}

void Application::processInput() {
    if (!_kbhit()) return;

    int ch = _getch();

    switch (ch) {
        case 'q':
        case 'Q':
        case 27:
            running_ = false;
            break;
        case '+':
        case '=':
            config_.asciiWidth += 10;
            converter_.setAsciiWidth(config_.asciiWidth);
            break;
        case '-':
            config_.asciiWidth = std::max(20, config_.asciiWidth - 10);
            converter_.setAsciiWidth(config_.asciiWidth);
            break;
        case '0':
            processor_.setFilter(FilterType::None);
            break;
        case '1':
            processor_.setFilter(FilterType::Grayscale);
            break;
        case '2':
            processor_.setFilter(FilterType::Invert);
            break;
        case '3':
            processor_.setFilter(FilterType::Threshold);
            break;
        case '4':
            processor_.setFilter(FilterType::Edge);
            break;
        case '5':
            processor_.setFilter(FilterType::Blur);
            break;
    }
}

void Application::updateFps() {
    frameCount_++;
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastFpsUpdate_).count();

    if (elapsed >= 1000) {
        fps_ = frameCount_ * 1000.0 / elapsed;
        frameCount_ = 0;
        lastFpsUpdate_ = now;
    }
}