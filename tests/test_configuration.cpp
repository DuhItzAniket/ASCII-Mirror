#include "Config.hpp"
#include <cassert>
#include <fstream>
#include <iostream>
#include <filesystem>

void test_default_config() {
    Config config;
    assert(config.cameraIndex == 0);
    assert(config.asciiWidth == 120);
    assert(config.charset == "@%#*+=-:. ");
    assert(config.invert == false);
    assert(config.showFps == true);
    assert(config.colorEnabled == false);
    assert(config.filterMode == "grayscale");
    assert(config.aspectCorrection == 0.5f);
    assert(config.targetFps == 30);
    std::cout << "test_default_config: PASSED\n";
}

void test_save_load() {
    Config config;
    config.cameraIndex = 1;
    config.asciiWidth = 80;
    config.charset = "█▓▒░";
    config.invert = true;
    config.showFps = false;
    config.filterMode = "edge";

    const std::string testFile = "test_config.cfg";
    config.saveToFile(testFile);

    Config loaded = Config::loadFromFile(testFile);
    assert(loaded.cameraIndex == 1);
    assert(loaded.asciiWidth == 80);
    assert(loaded.charset == "█▓▒░");
    assert(loaded.invert == true);
    assert(loaded.showFps == false);
    assert(loaded.filterMode == "edge");

    std::filesystem::remove(testFile);
    std::cout << "test_save_load: PASSED\n";
}

void test_load_nonexistent() {
    Config config = Config::loadFromFile("nonexistent.cfg");
    assert(config.cameraIndex == 0);
    assert(config.asciiWidth == 120);
    std::cout << "test_load_nonexistent: PASSED\n";
}

void test_from_args_camera() {
    char* argv[] = { const_cast<char*>("test"), const_cast<char*>("-c"), const_cast<char*>("2") };
    Config config = Config::fromArgs(3, argv);
    assert(config.cameraIndex == 2);
    std::cout << "test_from_args_camera: PASSED\n";
}

void test_from_args_width() {
    char* argv[] = { const_cast<char*>("test"), const_cast<char*>("-w"), const_cast<char*>("60") };
    Config config = Config::fromArgs(3, argv);
    assert(config.asciiWidth == 60);
    std::cout << "test_from_args_width: PASSED\n";
}

void test_from_args_charset() {
    char* argv[] = { const_cast<char*>("test"), const_cast<char*>("--charset"), const_cast<char*>("█▓▒░") };
    Config config = Config::fromArgs(3, argv);
    assert(config.charset == "█▓▒░");
    std::cout << "test_from_args_charset: PASSED\n";
}

void test_from_args_invert() {
    char* argv[] = { const_cast<char*>("test"), const_cast<char*>("--invert") };
    Config config = Config::fromArgs(2, argv);
    assert(config.invert == true);
    std::cout << "test_from_args_invert: PASSED\n";
}

void test_from_args_filter() {
    char* argv[] = { const_cast<char*>("test"), const_cast<char*>("--filter"), const_cast<char*>("edge") };
    Config config = Config::fromArgs(3, argv);
    assert(config.filterMode == "edge");
    std::cout << "test_from_args_filter: PASSED\n";
}

int main() {
    std::cout << "Running Config tests...\n\n";

    test_default_config();
    test_save_load();
    test_load_nonexistent();
    test_from_args_camera();
    test_from_args_width();
    test_from_args_charset();
    test_from_args_invert();
    test_from_args_filter();

    std::cout << "\nAll tests passed!\n";
    return 0;
}