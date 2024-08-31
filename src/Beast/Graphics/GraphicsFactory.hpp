#pragma once
#include "Beast/Graphics/IGraphics.hpp"

#include "Beast/Windows/Window.hpp"
#include "Beast/Common/Helpers.hpp"
#include "Beast/Common/Types.hpp"

namespace be::internals
{
    Unique<graphics::IGraphics> CreateGraphics(graphics::RenderingApi api, const Window& window);
} // namespace be::internals
