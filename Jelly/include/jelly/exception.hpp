#pragma once

#include "jelly/core/logger.hpp"

#include <stdexcept>

namespace jelly {

/// Exception class representing errors related to the graphics API.
/// Logs the error message upon construction.
class Exception final : public std::runtime_error {
public:
    explicit Exception(const std::string& msg) : std::runtime_error(msg) {
        jelly::core::Logger::Log(jelly::core::LogLevel::Error, msg);
    }
};

}