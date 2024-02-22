#pragma once
#include "Beast/Graphics/Graphics.h"
#include "Beast/Graphics/D3D11/D3D11.h"
#include "Beast/Graphics/D3D11/VertexBuffer.h"
#include "Beast/Graphics/D3D11/VertexShader.h"
#include "Beast/Graphics/D3D11/PixelShader.h"

#include "Beast/Windows/IWindow.h"

namespace be::graphics::d3d11
{
    // TODO: Could we combine this into a single class like this?
    /**
     * class Graphics final : public IGraphics, public IRenderContext
     * {
     * private:
     * d3d11::Device m_device;
     * d3d11::Context m_context;
     * }
     */

    class Device final : public IGraphics
    {
    public:
        Device(const IWindow& window);
        ~Device() override;
        
        /*wrl::ComPtr<ID3D11DeviceContext> GetContext() const noexcept;
        wrl::ComPtr<IDXGISwapChain> GetSwapChain() const noexcept;
        wrl::ComPtr<ID3D11RenderTargetView> CreateRenderTargetView(IDXGISwapChain& swapChain) const;*/

        // Public API
        graphics::VertexBuffer CreateVertexBuffer(uint32 stride, uint32 maxSize) override;
        graphics::VertexShader CreateVertexShader(const FilesystemPath& filepath, const InputLayout& inputLayout) override;
        graphics::PixelShader CreatePixelShader(const FilesystemPath& filepath) override;

        const IRenderContext& GetContext() const noexcept;

        // Internal API
        wrl::ComPtr<ID3D11RenderTargetView> CreateRenderTargetView(IDXGISwapChain& swapChain) const;
        
        const d3d11::VertexBuffer& GetBuffer(graphics::VertexBuffer bufferRef) const;
        const d3d11::VertexShader& GetShader(graphics::VertexShader shaderRef) const;
        const d3d11::PixelShader& GetShader(graphics::PixelShader shaderRef) const;

    private:
        std::unordered_map<Id, d3d11::VertexBuffer> m_vertexBuffers;
        std::unordered_map<Id, d3d11::VertexShader> m_vertexShaders;
        std::unordered_map<Id, d3d11::PixelShader> m_pixelShaders;

        wrl::ComPtr<ID3D11Device> m_device = nullptr;
        wrl::ComPtr<IDXGISwapChain> m_swapChain = nullptr;
        wrl::ComPtr<ID3D11DeviceContext> m_context = nullptr;

        // TEMP
        wrl::ComPtr<ID3D11Texture2D> m_bufferPtr;
        wrl::ComPtr<ID3D11DepthStencilView> m_view;

        Unique<IRenderContext> m_renderContext;
    };
}
