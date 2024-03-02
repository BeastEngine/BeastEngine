#pragma once
#include "Beast/Graphics/IGraphics.h"

#include "Beast/Windows/Window.h"
#include "Beast/Common/Helpers.h"
#include "Beast/Common/Types.h"

namespace be::internals
{
    Unique<graphics::IGraphics> CreateGraphics(graphics::RenderingApi api, const Window& window);
} // namespace be::internals
