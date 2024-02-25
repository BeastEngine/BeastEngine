#pragma once
#include "Beast/Graphics/D3D11/D3D11.h"

namespace be::graphics::d3d11
{
    class RenderTarget
    {
    public:
        explicit RenderTarget(wrl::ComPtr<ID3D11RenderTargetView> view) noexcept
            : m_view(std::move(view))
        {}

        ID3D11RenderTargetView* const* Address() const
        {
            return m_view.GetAddressOf();
        }

        ID3D11RenderTargetView* View() const
        {
            return m_view.Get();
        }

    private:
        wrl::ComPtr<ID3D11RenderTargetView> m_view;
    };
} // namespace be::graphics::d3d11
