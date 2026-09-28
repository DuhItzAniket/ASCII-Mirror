#include "Application.hpp"
#include "Config.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    try {
        Config config = Config::fromArgs(argc, argv);
        Application app(config);
        return app.run();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown fatal error occurred" << std::endl;
        return 1;
    }
}