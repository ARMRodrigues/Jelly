#pragma once

#include <functional>
#include <ranges>

namespace jelly::core {
///  <summary>
/// A RAII wrapper for managing external resources (e.g. OpenGL handles, file descriptors).
/// Cleans up the resource automatically unless released manually.
/// </summary>
template<typename T>
class ManagedResource {
public:
    ManagedResource() = default;

    /// <summary>
    /// Creates a managed resource with a cleanup function and an optional invalid value (defaults to T{}).
    /// </summary>
    ManagedResource(T resource, std::function<void(T)> cleanupFunc, T invalid = T{})
        : resource_(resource), cleanupFunc_(std::move(cleanupFunc)), invalid_(invalid) {}

    ~ManagedResource() {
        if (cleanupFunc_ && resource_ != invalid_) {
            cleanupFunc_(resource_);
        }
    }

    ManagedResource(const ManagedResource&) = delete;
    ManagedResource& operator=(const ManagedResource&) = delete;

    ManagedResource& operator=(ManagedResource&& other) noexcept {
        if (this != &other) {
            reset();
            swap(other);
        }
        return *this;
    }

    T& ref() { return resource_; }
    const T& ref() const { return resource_; }

    /// <summary>
    /// Resets the managed resource, invoking the cleanup function if necessary.
    /// </summary>
    void reset() {
        if (cleanupFunc_ && resource_ != invalid_) {
            cleanupFunc_(resource_);
        }
        resource_ = invalid_;
        cleanupFunc_ = nullptr;
    }

    /// <summary>
    /// Swaps this resource with another.
    /// </summary>
    void swap(ManagedResource& other) noexcept {
        std::ranges::swap(resource_, other.resource_);
        std::ranges::swap(invalid_, other.invalid_);
        std::ranges::swap(cleanupFunc_, other.cleanupFunc_);
    }

    /// <summary>
    /// Releases ownership of the managed resource without invoking the cleanup function.
    /// </summary>
    /// <returns>The raw resource. After calling this, the object no longer manages the resource.</returns>
    T release() {
        T temp = resource_;
        resource_ = invalid_;
        cleanupFunc_ = nullptr;
        return temp;
    }

    /// <summary>
    /// Returns the managed resource.
    /// </summary>
    T get() const { return resource_; }

    /// <summary>
    /// Implicit conversion to the underlying resource.
    /// </summary>
    operator T() const { return resource_; }

private:
    T resource_{};
    T invalid_{};
    std::function<void(T)> cleanupFunc_;
};

}
