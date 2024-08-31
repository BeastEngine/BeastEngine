#pragma once
#include "Beast/Graphics/IGraphics.hpp"
#include "Beast/Graphics/Types.hpp"

#include "Beast/Debug.hpp"
#include "Beast/Math/Types.hpp"

namespace be::graphics
{
    class Renderer2D final
    {
        struct Primitive
        {
            Vec2 position;
            Color color;
        };

    public:
        explicit Renderer2D(be::Unique<IGraphics> graphics, uint32 maxNumberOfSprites);

        void StartFrame();
        void AddSprite(const Vec2& position, const Color& color);
        void EndFrame(const Viewport& viewport);

    private:
        be::Unique<IGraphics> m_graphics;
        std::vector<Primitive> m_framePrimitives;

        bool m_hasFrameStarted = false;
        bool m_hasFrameEnded = true;

        // TEMP
        VertexBuffer m_buffer;
        VertexShader m_vertexShader;
        PixelShader m_pixelShader;
    };
} // namespace be::graphics
