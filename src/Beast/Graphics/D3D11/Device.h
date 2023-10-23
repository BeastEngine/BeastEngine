#pragma once
#include "Beast/Graphics/Graphics.h"
#include "Beast/Graphics/D3D11/D3D11.h"
#include "Beast/Graphics/D3D11/VertexBuffer.h"

#include "Beast/Windows/IWindow.h"

namespace be::graphics::d3d11
{
    class Device final : public IDevice
    {
    public:
        Device(const IWindow& window);
        wrl::ComPtr<ID3D11DeviceContext> GetContext() const noexcept;
        wrl::ComPtr<IDXGISwapChain> GetSwapChain() const noexcept;
        wrl::ComPtr<ID3D11RenderTargetView> CreateRenderTargetView(IDXGISwapChain& swapChain) const;

    private:
        graphics::VertexBuffer CreateVertexBuffer() override;

    private:
        std::unordered_map<Id, d3d11::VertexBuffer> m_vertexBuffers;

        wrl::ComPtr<ID3D11Device> m_device = nullptr;
        wrl::ComPtr<IDXGISwapChain> m_swapChain = nullptr;
        wrl::ComPtr<ID3D11DeviceContext> m_context = nullptr;
    };
}
