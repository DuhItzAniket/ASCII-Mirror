#pragma once

#include <string>

class TerminalRenderer {
public:
    TerminalRenderer();
    ~TerminalRenderer();

    void initialize();
    void render(const std::string& frame);
    void shutdown();
    void clear();
    void moveCursorHome();

    int getTerminalWidth() const;
    int getTerminalHeight() const;

    bool isInitialized() const { return initialized_; }

private:
    bool initialized_ = false;
    int terminalWidth_ = 80;
    int terminalHeight_ = 24;
    std::string lastFrame_;

    void getTerminalSize();
    void enableAnsiSupport();
};