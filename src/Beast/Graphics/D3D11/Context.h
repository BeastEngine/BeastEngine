#pragma once
#include <Beast/Graphics/IContext.h>

#include <Beast/Windows/IWindow.h>
#include <Beast/PlatformSetup.h>

#include <d3d11.h>
#include <wrl.h>

namespace be::graphics::d3d11
{
    namespace wrl = Microsoft::WRL;

    class Context : public IContext
    {
    public:
        explicit Context(const IWindow& window);
        
        void ClearRenderTargetView(const Color& color) const noexcept override;
        void Present() const noexcept override;

    private:
        wrl::ComPtr<ID3D11Device> m_device = nullptr;
        wrl::ComPtr<IDXGISwapChain> m_swapChain = nullptr;
        wrl::ComPtr<ID3D11DeviceContext> m_context = nullptr;
        wrl::ComPtr<ID3D11RenderTargetView> m_renderTargetView = nullptr;
    };
} // namespace be::graphics::d3d11
