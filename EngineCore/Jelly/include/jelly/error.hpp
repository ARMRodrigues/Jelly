#pragma once

#include <iostream>
#include <exception>

namespace jelly {

/**
 * @brief Utility class for displaying engine errors to stderr in a consistent format.
 */
class Error {
public:
    static void Print(const std::exception& e) {
        std::cerr << "[Engine Error] " << e.what() << std::endl;
    }

    static void Print(const std::string& message) {
        std::cerr << "[Engine Error] " << message << std::endl;
    }
};

}
