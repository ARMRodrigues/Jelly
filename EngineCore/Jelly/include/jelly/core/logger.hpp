#pragma once

#include <string>

namespace jelly::core {

/// @brief Logging severity levels.
enum class LogLevel {
    Debug,      // Debug messages for development.
    Info,       // Standard informational messages (e.g., startup).
    Highlight,  // Highlighted info (e.g., important status).
    Warning,    // Warnings about potential issues.
    Error       // Errors that require attention.
};

/// Simple logging utility with platform-specific colored output.
class Logger {
public:
    /// @brief Logs a message with a specified severity level.
    /// @param level The severity level of the log.
    /// @param message The message to display.
    static void Log(LogLevel level, const std::string& message);

    /// @brief Logs a info message.
    /// @param message The message to display.
    static void Log(const std::string& message);
};

}
