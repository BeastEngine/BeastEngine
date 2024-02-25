#pragma once
#include "Beast/PlatformSetup.h"
#include "Beast/Graphics/D3D11/Debug.h"

#include <d3d11.h>
#include <wrl.h>

namespace be::graphics::d3d11
{
    namespace wrl = Microsoft::WRL;

    class API
    {
    public:
        API(wrl::ComPtr<ID3D11Device> device, wrl::ComPtr<IDXGISwapChain> swapChain, wrl::ComPtr<ID3D11DeviceContext> context) noexcept
            : m_device(std::move(device)), m_swapChain(std::move(swapChain)), m_context(std::move(context))
        {}

        ID3D11Device& Device() noexcept
        {
            return *m_device.Get();
        }

        IDXGISwapChain& SwapChain() noexcept
        {
            return *m_swapChain.Get();
        }

        ID3D11DeviceContext& Context() noexcept
        {
            return *m_context.Get();
        }

    private:
        wrl::ComPtr<ID3D11Device> m_device = nullptr;
        wrl::ComPtr<IDXGISwapChain> m_swapChain = nullptr;
        wrl::ComPtr<ID3D11DeviceContext> m_context = nullptr;
    };
}
