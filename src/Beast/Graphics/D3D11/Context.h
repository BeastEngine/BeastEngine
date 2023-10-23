#pragma once
#include "Beast/Graphics/Graphics.h"
#include "Beast/Graphics/D3D11/D3D11.h"
#include "Beast/Graphics/D3D11/VertexBuffer.h"
#include "Beast/Graphics/D3D11/Device.h"

namespace be::graphics::d3d11
{
    class Context : public IContext
    {
    public:
        Context(Device& device);
        
        void Clear(const Color& color) const noexcept override;
        void Present() const noexcept override;

    private:
        Device& m_device;

        wrl::ComPtr<IDXGISwapChain> m_swapChain = nullptr;
        wrl::ComPtr<ID3D11DeviceContext> m_context = nullptr;
        wrl::ComPtr<ID3D11RenderTargetView> m_renderTargetView = nullptr;
    };
} // namespace be::graphics::d3d11
