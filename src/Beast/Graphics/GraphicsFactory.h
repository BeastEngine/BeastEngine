#pragma once
#include "Beast/Graphics/Graphics.h"

#include "Beast/Windows/IWindow.h"
#include "Beast/Common/Helpers.h"
#include "Beast/Common/Types.h"

namespace be::internals
{
    Unique<graphics::IGraphics> CreateGraphics(graphics::RenderingApi api, const IWindow& window);
} // namespace be::graphics
