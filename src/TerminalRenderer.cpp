#include "TerminalRenderer.hpp"
#include <iostream>
#include <windows.h>

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