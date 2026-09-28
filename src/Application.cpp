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

    // Apply charset preset if specified (overrides custom charset)
    if (config.charsetPreset >= 0 && config.charsetPreset <= 3) {
        converter_.setCharset(static_cast<AsciiConverter::PresetCharset>(config.charsetPreset));
    }

    if (config.filterMode == "invert") processor_.setFilter(FilterType::Invert);
    else if (config.filterMode == "threshold") processor_.setFilter(FilterType::Threshold);
    else if (config.filterMode == "edge") processor_.setFilter(FilterType::Edge);
    else if (config.filterMode == "blur") processor_.setFilter(FilterType::Blur);
    else processor_.setFilter(FilterType::Grayscale);

    // Set color mode
    if (config.colorEnabled) {
        renderer_.setColorMode(TerminalRenderer::ColorMode::TrueColor);
    }
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
    std::cout << "  0         No filter\n";
    std::cout << "  C         Cycle charset preset\n";
    std::cout << "  K         Toggle color mode\n\n";

    std::cout << "Starting camera...\n";

    if (!camera_.open(config_.cameraIndex)) {
        std::cerr << "Failed to open camera. Exiting.\n";
        return 1;
    }

    std::cout << "Camera opened: " << camera_.getWidth() << "x" << camera_.getHeight()
              << " @ " << camera_.getFPS() << " FPS\n\n";

    renderer_.initialize();

    running_ = true;
    auto frameStart = std::chrono::steady_clock::now();
    const auto frameDuration = std::chrono::milliseconds(1000 / config_.targetFps);

    while (running_) {
        frameStart = std::chrono::steady_clock::now();

        if (!camera_.read(frameBuffer_) || frameBuffer_.empty()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }

        if (config_.colorEnabled && renderer_.getColorMode() != TerminalRenderer::ColorMode::None) {
            // Use color rendering directly from original frame
            cv::Mat processed = processor_.process(frameBuffer_);
            renderer_.renderColor(frameBuffer_, config_.asciiWidth, config_.aspectCorrection);
        } else {
            cv::Mat processed = processor_.process(frameBuffer_);
            std::string ascii = converter_.convert(processed);

            if (config_.showFps) {
                ascii += "\nFPS: " + std::to_string(static_cast<int>(fps_));
                auto stats = converter_.getStats();
                if (stats.framesProcessed > 0) {
                    double avgConvertMs = stats.convertTime.count() / 1000.0 / stats.framesProcessed;
                    double avgPreprocessMs = stats.preprocessTime.count() / 1000.0 / stats.framesProcessed;
                    ascii += " | Convert: " + std::to_string(static_cast<int>(avgConvertMs * 10) / 10.0) + "ms";
                    ascii += " | Preprocess: " + std::to_string(static_cast<int>(avgPreprocessMs * 10) / 10.0) + "ms";
                }
            }

            renderer_.render(ascii);
        }
        processInput();
        updateFps();

        auto elapsed = std::chrono::steady_clock::now() - frameStart;
        if (elapsed < frameDuration) {
            std::this_thread::sleep_for(frameDuration - elapsed);
        }
    }

    renderer_.shutdown();
    printStats();
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
        case 'c':
        case 'C':
            config_.charsetPreset = (config_.charsetPreset + 1) % 4;
            converter_.setCharset(static_cast<AsciiConverter::PresetCharset>(config_.charsetPreset));
            break;
        case 'k':
        case 'K':
            config_.colorEnabled = !config_.colorEnabled;
            if (config_.colorEnabled) {
                renderer_.setColorMode(TerminalRenderer::ColorMode::TrueColor);
            } else {
                renderer_.setColorMode(TerminalRenderer::ColorMode::None);
            }
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

void Application::printStats() const {
    auto stats = converter_.getStats();
    if (stats.framesProcessed > 0) {
        double avgConvertMs = stats.convertTime.count() / 1000.0 / stats.framesProcessed;
        double avgPreprocessMs = stats.preprocessTime.count() / 1000.0 / stats.framesProcessed;
        std::cout << "\nPerformance Stats:\n";
        std::cout << "  Frames processed: " << stats.framesProcessed << "\n";
        std::cout << "  Avg convert time: " << avgConvertMs << " ms\n";
        std::cout << "  Avg preprocess time: " << avgPreprocessMs << " ms\n";
        std::cout << "  Current FPS: " << fps_ << "\n";
    }
}