#include "jelly/graphics/graphic_context.hpp"

namespace jelly::graphics {

GraphicContext& GraphicContext::get() {
    static GraphicContext instance;
    return instance;
}

void GraphicContext::initialize(core::GraphicAPIType apiType, GraphicAPIInterface* api) {
    apiType_ = apiType;
    api_ = api;
}

core::GraphicAPIType GraphicContext::getAPIType() const {
    return apiType_;
}

GraphicAPIInterface* GraphicContext::getAPI() {
    return api_;
}

} // namespace jelly::graphics
