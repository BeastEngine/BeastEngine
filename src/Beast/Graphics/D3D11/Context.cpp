#include "Beast/Graphics/D3D11/Context.h"
#include "Beast/Graphics/D3D11/Asserts.h"

namespace be::graphics::d3d11
{
    Context::Context(Device& device)
        : m_device(device)
    {
        m_context = m_device.GetContext();
        m_swapChain = m_device.GetSwapChain();
        m_renderTargetView = m_device.CreateRenderTargetView(*m_swapChain.Get());
    }

    void graphics::d3d11::Context::Clear(const Color& color) const noexcept
    {
        m_context->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), nullptr);
        m_context->ClearRenderTargetView(m_renderTargetView.Get(), color.Data());
    }

    void graphics::d3d11::Context::Present() const noexcept
    {
        Assert(m_swapChain->Present(1, 0));
    }

    void be::graphics::d3d11::Context::Run()
    {
    }
} // namespace be::graphics::d3d11
