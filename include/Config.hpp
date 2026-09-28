#pragma once

#include <string>
#include <optional>

struct Config {
    int cameraIndex = 0;
    int asciiWidth = 120;
    std::string charset = "@%#*+=-:. ";
    bool invert = false;
    bool showFps = true;
    bool colorEnabled = false;
    std::string filterMode = "grayscale";
    float aspectCorrection = 0.5f;
    int targetFps = 30;

    static Config loadFromFile(const std::string& path);
    void saveToFile(const std::string& path) const;
    static Config fromArgs(int argc, char* argv[]);
    static void printUsage(const char* programName);
};