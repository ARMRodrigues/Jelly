#pragma once

#include <string>

namespace jelly::core {

/// Logging severity levels.
enum class LogLevel {
    Info,       ///< Standard informational messages (e.g., startup).
    Highlight,  ///< Highlighted info (e.g., important status).
    Warning,    ///< Warnings about potential issues.
    Error       ///< Errors that require attention.
};

/// Simple logging utility with platform-specific colored output.
class Logger {
public:
    /// Logs a message with a specified severity level.
    /// @param level The severity level of the log.
    /// @param message The message to display.
    static void Log(LogLevel level, const std::string& message);

    /// Logs a info message.
    /// @param message The message to display.
    static void Log(const std::string& message);
};

}
