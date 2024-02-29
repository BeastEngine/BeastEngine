#pragma once
#include "Beast/Graphics/IGraphics.h"
#include "Beast/Graphics/D3D11/D3D11.h"
#include "Beast/Graphics/D3D11/Buffer.h"
#include "Beast/Graphics/D3D11/Shader.h"
#include "Beast/Graphics/D3D11/RenderTarget.h"

#include "Beast/Windows/IWindow.h"

namespace be::graphics::d3d11
{
    class Graphics final : public IGraphics
    {
    public:
        explicit Graphics(const IWindow& window);

    private:
        graphics::VertexBuffer CreateVertexBuffer(uint32 stride, uint32 maxSize) override;
        graphics::VertexShader CreateVertexShader(const FilesystemPath& filepath, const InputLayout& inputLayout) override;
        graphics::PixelShader CreatePixelShader(const FilesystemPath& filepath) override;

        void UpdateVertexBuffer(graphics::VertexBuffer buffer, std::span<const Vertex> verticies) override;
        
        void Draw(const Pipeline& pipeline) override;
        void Clear(const Color& color) override;
        void Present() override;

    private:
        d3d11::API m_api;
        d3d11::RenderTarget m_renderTarget;

        std::vector<d3d11::VertexBuffer> m_vertexBuffers;
        std::vector<d3d11::VertexShader> m_vertexShaders;
        std::vector<d3d11::PixelShader> m_pixelShaders;
    };
} // namespace be::graphics::d3d11
