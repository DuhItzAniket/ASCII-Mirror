#include "Config.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstring>

Config Config::loadFromFile(const std::string& path) {
    Config config;
    std::ifstream file(path);
    if (!file.is_open()) {
        return config;
    }

    std::string line;
    while (std::getline(file, line)) {
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);

        if (line.empty() || line[0] == '#') continue;

        size_t eqPos = line.find('=');
        if (eqPos == std::string::npos) continue;

        std::string key = line.substr(0, eqPos);
        std::string value = line.substr(eqPos + 1);

        key.erase(0, key.find_first_not_of(" \t"));
        key.erase(key.find_last_not_of(" \t") + 1);
        value.erase(0, value.find_first_not_of(" \t"));
        value.erase(value.find_last_not_of(" \t") + 1);

        if (key == "camera_index") config.cameraIndex = std::stoi(value);
        else if (key == "ascii_width") config.asciiWidth = std::stoi(value);
        else if (key == "charset") config.charset = value;
        else if (key == "charset_preset") config.charsetPreset = std::stoi(value);
        else if (key == "invert") config.invert = (value == "true" || value == "1");
        else if (key == "show_fps") config.showFps = (value == "true" || value == "1");
        else if (key == "color_enabled") config.colorEnabled = (value == "true" || value == "1");
        else if (key == "filter_mode") config.filterMode = value;
        else if (key == "aspect_correction") config.aspectCorrection = std::stof(value);
        else if (key == "target_fps") config.targetFps = std::stoi(value);
    }

    return config;
}

void Config::saveToFile(const std::string& path) const {
    std::ofstream file(path);
    if (!file.is_open()) return;

    file << "# ASCII Mirror Configuration\n";
    file << "camera_index=" << cameraIndex << "\n";
    file << "ascii_width=" << asciiWidth << "\n";
    file << "charset=" << charset << "\n";
    file << "charset_preset=" << charsetPreset << "\n";
    file << "invert=" << (invert ? "true" : "false") << "\n";
    file << "show_fps=" << (showFps ? "true" : "false") << "\n";
    file << "color_enabled=" << (colorEnabled ? "true" : "false") << "\n";
    file << "filter_mode=" << filterMode << "\n";
    file << "aspect_correction=" << aspectCorrection << "\n";
    file << "target_fps=" << targetFps << "\n";
}

Config Config::fromArgs(int argc, char* argv[]) {
    Config config;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-h" || arg == "--help") {
            printUsage(argv[0]);
            exit(0);
        } else if (arg == "-c" || arg == "--camera") {
            if (i + 1 < argc) config.cameraIndex = std::stoi(argv[++i]);
        } else if (arg == "-w" || arg == "--width") {
            if (i + 1 < argc) config.asciiWidth = std::stoi(argv[++i]);
        } else if (arg == "--charset") {
            if (i + 1 < argc) config.charset = argv[++i];
        } else if (arg == "--charset-preset") {
            if (i + 1 < argc) config.charsetPreset = std::stoi(argv[++i]);
        } else if (arg == "--invert") {
            config.invert = true;
        } else if (arg == "--no-fps") {
            config.showFps = false;
        } else if (arg == "--color") {
            config.colorEnabled = true;
        } else if (arg == "--filter") {
            if (i + 1 < argc) config.filterMode = argv[++i];
        } else if (arg == "--config") {
            if (i + 1 < argc) config = loadFromFile(argv[++i]);
        }
    }

    return config;
}

void Config::printUsage(const char* programName) {
    std::cout << "Usage: " << programName << " [options]\n\n";
    std::cout << "Options:\n";
    std::cout << "  -h, --help           Show this help message\n";
    std::cout << "  -c, --camera <idx>   Camera index (default: 0)\n";
    std::cout << "  -w, --width <cols>   ASCII width in characters (default: 120)\n";
    std::cout << "  --charset <chars>    ASCII character ramp (default: @%#*+=-:. )\n";
    std::cout << "  --charset-preset <n> Preset charset: 0=Standard, 1=Dense, 2=Blocks, 3=Minimal\n";
    std::cout << "  --invert             Invert brightness mapping\n";
    std::cout << "  --no-fps             Disable FPS display\n";
    std::cout << "  --color              Enable color output (experimental)\n";
    std::cout << "  --filter <mode>      Filter mode: grayscale, invert, threshold, edge, blur\n";
    std::cout << "  --config <file>      Load configuration from file\n";
    std::cout << "\nKeyboard controls during runtime:\n";
    std::cout << "  Q, ESC    Quit\n";
    std::cout << "  +, =      Increase ASCII width\n";
    std::cout << "  -         Decrease ASCII width\n";
    std::cout << "  1         Grayscale filter\n";
    std::cout << "  2         Invert filter\n";
    std::cout << "  3         Threshold filter\n";
    std::cout << "  4         Edge filter\n";
    std::cout << "  5         Blur filter\n";
    std::cout << "  0         No filter\n";
}