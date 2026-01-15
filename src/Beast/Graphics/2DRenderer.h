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
    class Camera2D;
    class IGraphics;

    class Renderer2D final
    {
        struct Primitive
        {
            Vec2 position;
            Sprite sprite;
        };

    public:
        explicit Renderer2D(be::Unique<IGraphics> graphics, uint32 maxNumberOfSprites, be::Unique<fs::ResourcesManager> resourcesManager);
        ~Renderer2D();

        BE_IMPLEMENT_ADDITIONAL_CONSTRUCTORS_DEFAULT(Renderer2D);

        void StartFrame();
        void AddSprite(const Vec2& position, const Sprite& sprite);
        void EndFrame(const Camera2D& camera);

    private:
        Texture GetOrCreateTexture(TextureId id);

    private:
        be::Unique<IGraphics> m_graphics;
        be::Unique<fs::ResourcesManager> m_resourcesManager;
        std::unordered_map<TextureId, std::vector<Primitive>, TextureId::Hasher> m_framePrimitives;

        std::unordered_map<TextureId, Texture, TextureId::Hasher> m_textures;

        BE_DEBUG_FIELD(bool m_hasFrameStarted = false);

        // TEMP
        VertexBuffer m_buffer;
        VertexShader m_vertexShader;
        PixelShader m_pixelShader;
        ConstantBuffer m_cameraCBuffer;
    };
} // namespace be::graphics
