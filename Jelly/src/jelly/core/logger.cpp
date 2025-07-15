#include "jelly/core/logger.hpp"

#include <iostream>
#include <iomanip>
#include <chrono>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

namespace jelly::core {

// -----------------------------------------------------------------------------
// Returns the current system time in HH:MM:SS format.
// -----------------------------------------------------------------------------
static std::string CurrentTimeHHMMSS() {
    using namespace std::chrono;
    auto t  = system_clock::to_time_t(system_clock::now());
    std::tm tm;
#if defined(_WIN32)
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif
    std::ostringstream oss;
    oss << std::put_time(&tm, "%H:%M:%S");
    return oss.str();
}

// -----------------------------------------------------------------------------
// Logs a message with platform-specific colored output and timestamp.
// -----------------------------------------------------------------------------
void Logger::Log(LogLevel level, const std::string& message) {
    const std::string timeStr = CurrentTimeHHMMSS();

#if defined(_WIN32) || defined(_WIN64)
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    WORD color;

    switch (level) {
        case LogLevel::Info:        color = FOREGROUND_GREEN | FOREGROUND_INTENSITY; break;
        case LogLevel::Highlight:   color = FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY; break;
        case LogLevel::Warning:     color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY; break;
        case LogLevel::Error:       color = FOREGROUND_RED | FOREGROUND_INTENSITY; break;
        default:                    color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; break;
    }

    SetConsoleTextAttribute(hConsole, color);

    std::cout << "[" << timeStr << "] ";

    switch (level) {
        case LogLevel::Info:        std::cout << "[INFO] "; break;
        case LogLevel::Highlight:   std::cout << "[INFO] "; break;
        case LogLevel::Warning:     std::cout << "[WARN] "; break;
        case LogLevel::Error:       std::cout << "[ERROR] "; break;
        default:                    std::cout << "[LOG] "; break;
    }

    std::cout << message << std::endl;

    // Reset color
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#else
    const char* colorCode;
    switch (level) {
        case LogLevel::Info:        colorCode = "\033[32m"; break; // Green
        case LogLevel::Highlight:   colorCode = "\033[38;2;122;44;189m"; break;
        case LogLevel::Warning:     colorCode = "\033[33m"; break; // Yellow
        case LogLevel::Error:       colorCode = "\033[31m"; break; // Red
        default:                    colorCode = "\033[0m"; break;
    }

    const char* levelStr;
    switch (level) {
        case LogLevel::Info:        levelStr = "[INFO] "; break;
        case LogLevel::Highlight:   levelStr = "[INFO] "; break;
        case LogLevel::Warning:     levelStr = "[WARN] "; break;
        case LogLevel::Error:       levelStr = "[ERROR] "; break;
        default:                    levelStr = "[LOG] "; break;
    }

    std::cout << colorCode << "[" << timeStr << "] " << levelStr << message << "\033[0m" << std::endl;
#endif
}

}