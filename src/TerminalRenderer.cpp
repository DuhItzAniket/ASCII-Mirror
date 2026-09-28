#include "TerminalRenderer.hpp"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <windows.h>
#include <sstream>
#include <iomanip>

TerminalRenderer::TerminalRenderer() = default;

TerminalRenderer::~TerminalRenderer() {
    shutdown();
}

void TerminalRenderer::initialize() {
    if (initialized_) return;

    enableAnsiSupport();
    getTerminalSize();
    clear();
    initialized_ = true;
}

void TerminalRenderer::render(const std::string& frame) {
    if (!initialized_) {
        initialize();
    }

    moveCursorHome();
    std::cout << frame << std::flush;
    lastFrame_ = frame;
}

void TerminalRenderer::renderColor(const cv::Mat& frame, int asciiWidth, float aspectCorrection) {
    if (!initialized_) {
        initialize();
    }

    if (frame.empty()) return;

    cv::Mat rgb;
    if (frame.channels() == 1) {
        cv::cvtColor(frame, rgb, cv::COLOR_GRAY2BGR);
    } else if (frame.channels() == 3) {
        rgb = frame.clone();
    } else if (frame.channels() == 4) {
        cv::cvtColor(frame, rgb, cv::COLOR_BGRA2BGR);
    } else {
        return;
    }

    int asciiHeight = static_cast<int>(rgb.rows * aspectCorrection *
        static_cast<float>(asciiWidth) / rgb.cols);
    if (asciiHeight <= 0) asciiHeight = 1;

    moveCursorHome();

    const float rowScale = static_cast<float>(rgb.rows) / asciiHeight;
    const float colScale = static_cast<float>(rgb.cols) / asciiWidth;

    std::ostringstream oss;

    for (int y = 0; y < asciiHeight; ++y) {
        const int srcY = std::min(static_cast<int>(y * rowScale), rgb.rows - 1);

        for (int x = 0; x < asciiWidth; ++x) {
            const int srcX = std::min(static_cast<int>(x * colScale), rgb.cols - 1);
            cv::Vec3b pixel = rgb.at<cv::Vec3b>(srcY, srcX);

            if (colorMode_ == ColorMode::Grayscale) {
                int gray = (pixel[0] + pixel[1] + pixel[2]) / 3;
                oss << "\x1b[38;5;" << (16 + (gray * 231 / 255)) << "m█";
            } else if (colorMode_ == ColorMode::TrueColor) {
                oss << "\x1b[38;2;" << static_cast<int>(pixel[2]) << ";"
                    << static_cast<int>(pixel[1]) << ";"
                    << static_cast<int>(pixel[0]) << "m█";
            }
        }
        oss << "\x1b[0m\n";
    }

    std::cout << oss.str() << std::flush;
}

void TerminalRenderer::shutdown() {
    if (!initialized_) return;

    std::cout << "\x1b[0m" << std::flush;
    initialized_ = false;
}

void TerminalRenderer::clear() {
    std::cout << "\x1b[2J" << std::flush;
}

void TerminalRenderer::moveCursorHome() {
    std::cout << "\x1b[H" << std::flush;
}

int TerminalRenderer::getTerminalWidth() const {
    return terminalWidth_;
}

int TerminalRenderer::getTerminalHeight() const {
    return terminalHeight_;
}

void TerminalRenderer::getTerminalSize() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        terminalWidth_ = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        terminalHeight_ = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    }
}

void TerminalRenderer::enableAnsiSupport() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return;

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}

std::string TerminalRenderer::pixelToAnsiColor(int r, int g, int b) const {
    if (colorMode_ == ColorMode::Grayscale) {
        int gray = (r + g + b) / 3;
        return "\x1b[38;5;" + std::to_string(16 + (gray * 231 / 255)) + "m";
    } else if (colorMode_ == ColorMode::TrueColor) {
        std::ostringstream oss;
        oss << "\x1b[38;2;" << r << ";" << g << ";" << b << "m";
        return oss.str();
    }
    return "";
}