#pragma once
#include "Beast/Graphics/Types.h"

#include "Beast/Debug.h"
#include "Beast/Common/Helpers.h"
#include "Beast/Math/Types.h"

#include <unordered_map>

namespace be::fs
{
    class ResourcesManager;
}

namespace be::graphics
{
    // TODO: Add resource manager that will return filepath for given TextureId

    class Camera2D;
    class IGraphics;

    class Renderer2D final
    {
        struct Primitive
        {
            Vec2 position;
            Color color;
        };

    public:
        explicit Renderer2D(be::Unique<IGraphics> graphics, uint32 maxNumberOfSprites, be::Unique<fs::ResourcesManager> resourcesManager);
        ~Renderer2D();

        BE_IMPLEMENT_ADDITIONAL_CONSTRUCTORS_DEFAULT(Renderer2D);

        void StartFrame();
        void AddSprite(const Vec2& position, const Sprite& sprite);
        void EndFrame(const Camera2D& camera);

    private:
        be::Unique<IGraphics> m_graphics;
        be::Unique<fs::ResourcesManager> m_resourcesManager;
        std::vector<Primitive>
            m_framePrimitives;

        std::unordered_map<TextureId, Texture, TextureId::Hasher> m_textures;

        bool m_hasFrameStarted = false;
        bool m_hasFrameEnded = true;

        // TEMP
        VertexBuffer m_buffer;
        VertexShader m_vertexShader;
        PixelShader m_pixelShader;
        ConstantBuffer m_cameraCBuffer;
        Texture m_texture;
    };
} // namespace be::graphics
