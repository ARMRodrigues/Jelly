#pragma once

#include "jelly/jelly_export.hpp"

#include <chrono>

namespace jelly::core {

class JELLY_EXPORT GameTime {
public:
    /// @brief Initializes the game time tracking system
    /// @note Must be called once at application startup
    static void initialize() {
        startTime_ = std::chrono::high_resolution_clock::now();
        lastFrameTime_ = startTime_;
        deltaTime_ = 0.0f;
        totalTime_ = 0.0f;
        frameCount_ = 0;
    }

    /// @brief Updates the time measurements for the current frame
    /// @note Should be called once per frame
    static void update() {
        auto now = std::chrono::high_resolution_clock::now();

        std::chrono::duration<float> delta = now - lastFrameTime_;
        std::chrono::duration<float> total = now - startTime_;

        deltaTime_ = delta.count();
        totalTime_ = total.count();
        frameCount_++;

        lastFrameTime_ = now;
    }

    /// @brief Gets the time elapsed since the last frame in seconds
    /// @return Delta time in seconds
    static float deltaTime() { return deltaTime_; }

    /// @brief Gets the total time elapsed since initialization in seconds
    /// @return Total time in seconds
    static float totalTime() { return totalTime_; }

    /// @brief Gets the total number of frames processed since initialization
    /// @return Frame count
    static long  frameCount() { return frameCount_; }

private:
    static inline std::chrono::high_resolution_clock::time_point startTime_;
    static inline std::chrono::high_resolution_clock::time_point lastFrameTime_;

    static inline float deltaTime_ = 0.0f;
    static inline float totalTime_ = 0.0f;
    static inline long frameCount_ = 0;
};

} // namespace jelly::core
