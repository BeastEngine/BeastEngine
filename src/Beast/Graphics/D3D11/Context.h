#pragma once
#include "Beast/Graphics/Graphics.h"
#include "Beast/Graphics/D3D11/D3D11.h"
#include "Beast/Graphics/D3D11/VertexBuffer.h"

namespace be::graphics::d3d11
{
    class Device;

    class Context : public IRenderContext
    {
    public:
        Context(Device& device, IDXGISwapChain* swapChain, ID3D11DeviceContext* context, wrl::ComPtr<ID3D11RenderTargetView> renderTargetView);

        void Draw(const Pipeline& pipeline) const noexcept override;
        void Clear(const Color& color) const noexcept override;
        void Present() const noexcept override;

        void UpdateVertexBuffer(graphics::VertexBuffer buffer, std::span<const Vertex> verticies) const override;

    private:
        Device& m_device;

        IDXGISwapChain* m_swapChain;
        ID3D11DeviceContext* m_context;
        wrl::ComPtr<ID3D11RenderTargetView> m_renderTargetView;
    };
} // namespace be::graphics::d3d11
