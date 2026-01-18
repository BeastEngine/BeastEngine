#pragma once
#include "Beast/Common/Types.h"
#include "Beast/Common/Id.h"
#include "Beast/Common/Limits.h"

#include "Beast/Math/Types.h"

namespace be::graphics
{
    class TextureId final : public Id
    {
    public:
        constexpr TextureId()
            : Id() {}

        constexpr explicit TextureId(std::string_view stringId)
            : Id(stringId) {}
    };

    struct Color
    {
        constexpr const float* Data() const
        {
            return &r;
        }

        float r = 1.0f;
        float g = 1.0f;
        float b = 1.0f;
        float a = 1.0f;
    }; // namespace be::graphics

    struct Sprite
    {
        Color color;
        TextureId texture;
    };

    struct Vertex
    {
        Vec2 position;
        Color color;
        Vec2 uv;
    };

    constexpr inline auto VERTEX_STRIDE = sizeof(Vertex);

    enum class RenderingApi
    {
        D3D11,
    };

    enum class VertexBuffer : uint8
    {
    };

    enum class ConstantBuffer : uint8
    {
    };

    struct InputLayout
    {
        struct VertexAttribute
        {
            enum class Format
            {
                Vec2,
                Vec4,
            };

            const char* name;
            Format format;
            uint32 index = 0u;
        };

        std::vector<VertexAttribute> vertexAttributes;
    };

    enum class VertexShader : uint8
    {
    };

    enum class PixelShader : uint8
    {
    };

    struct Viewport
    {
        Vec2 dimensions;
    };

    enum class Texture : uint16
    {
    };

    inline constexpr auto MAX_VB_INDEX = limits::MaxValue<VertexBuffer>();
    inline constexpr auto MAX_CB_INDEX = limits::MaxValue<ConstantBuffer>();
    inline constexpr auto MAX_VS_INDEX = limits::MaxValue<VertexShader>();
    inline constexpr auto MAX_PS_INDEX = limits::MaxValue<PixelShader>();
    inline constexpr auto MAX_TEXTURE_INDEX = limits::MaxValue<Texture>();

} // namespace be::graphics
