#pragma once
#include "Beast/Ecs/Components/Core.h"
#include "Beast/Ecs/Components/Graphics.h"

#include "Beast/Graphics/Graphics.h"
#include "Beast/Graphics/Types.h"

#include "Beast/Debug.h"

namespace be::graphics
{
    class Renderer2D
    {
        struct Primitive
        {
            Transform transform;
            Texture texture;
            Material material;
        };

    public:
        explicit Renderer2D(be::Unique<IGraphics> graphics);

        void StartFrame();
        void AddSprite(const Transform& transform, const Sprite& sprite);
        void EndFrame();

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
