#pragma once

#include "jelly/jelly_export.hpp"
#include "jelly/core/graphic_api_type.hpp"
#include "jelly/graphics/graphic_api_interface.hpp"

namespace jelly::graphics {

/// @brief Manages access to the current graphics API implementation.
///
/// This singleton class stores the selected graphics API type and
/// its corresponding interface implementation.
class JELLY_EXPORT GraphicContext {
public:
    /// @brief Access the singleton instance.
    /// @return A reference to the global GraphicContext instance.
    static GraphicContext& get();

    /// @brief Initializes the graphic context with the selected API.
    /// @param apiType The type of the graphics API being used.
    /// @param api A pointer to the API interface implementation.
    void initialize(core::GraphicAPIType apiType, GraphicAPIInterface* api);

    /// @brief Gets the currently selected graphics API type.
    /// @return The current GraphicAPIType.
    core::GraphicAPIType getAPIType() const;

    /// @brief Gets the generic graphics API interface.
    /// @return A pointer to the GraphicAPIInterface in use.
    GraphicAPIInterface* getAPI();

private:
    /// @brief Private constructor to enforce singleton behavior.
    GraphicContext() = default;

    core::GraphicAPIType apiType_ = core::GraphicAPIType::NoApi;
    GraphicAPIInterface* api_ = nullptr;
};

} // namespace jelly::graphics
