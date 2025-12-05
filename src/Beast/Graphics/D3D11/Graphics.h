#pragma once
#include "Beast/Graphics/IGraphics.h"
#include "Beast/Graphics/D3D11/D3D11.h"
#include "Beast/Graphics/D3D11/Buffer.h"
#include "Beast/Graphics/D3D11/Shader.h"
#include "Beast/Graphics/D3D11/RenderTarget.h"
#include "Beast/Graphics/D3D11/Textures.h"

#include "Beast/Windows/Window.h"

namespace be::graphics::d3d11
{
    class Graphics final : public IGraphics
    {
    public:
        explicit Graphics(const Window& window);

    private:
        graphics::VertexBuffer CreateVertexBuffer(uint32 stride, uint32 maxSize) override;
        graphics::ConstantBuffer CreateConstantBuffer(uint32 size) override;
        graphics::VertexShader CreateVertexShader(const fs::Path& filepath, const InputLayout& inputLayout) override;
        graphics::PixelShader CreatePixelShader(const fs::Path& filepath) override;
        graphics::Texture CreateTexture(const Image& textureData) override;

        void UpdateVertexBuffer(graphics::VertexBuffer buffer, std::span<const Vertex> verticies) override;
        void UpdateConstantBuffer(graphics::ConstantBuffer buffer, const void* const data) override;

        void Draw(const Pipeline& pipeline) override;
        void Clear(const Color& color) override;
        void Present() override;

    private:
        d3d11::API m_api;
        d3d11::RenderTarget m_renderTarget;

        std::vector<d3d11::VertexBuffer> m_vertexBuffers;
        std::vector<d3d11::ConstantBuffer> m_constantBuffers;
        std::vector<d3d11::VertexShader> m_vertexShaders;
        std::vector<d3d11::PixelShader> m_pixelShaders;
        std::vector<d3d11::Texture> m_textures;
    };
} // namespace be::graphics::d3d11
