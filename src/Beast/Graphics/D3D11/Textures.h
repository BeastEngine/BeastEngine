#pragma once
#include "Beast/Graphics/D3D11/D3D11.h"

#include "Beast/Common/Types.h"

namespace be::graphics::d3d11
{
    class Texture final
    {
    public:
        Texture(wrl::ComPtr<ID3D11ShaderResourceView> textureResourceView, wrl::ComPtr<ID3D11SamplerState> samplerState)
            : m_textureResourceView(std::move(textureResourceView)), m_samplerState(std::move(samplerState))
        {}

        ID3D11ShaderResourceView* const* ResourceViewAddress() const noexcept
        {
            return m_textureResourceView.GetAddressOf();
        }

        ID3D11SamplerState* const* SamplerAddress() const noexcept
        {
            return m_samplerState.GetAddressOf();
        }

    private:
        wrl::ComPtr<ID3D11ShaderResourceView> m_textureResourceView;
        wrl::ComPtr<ID3D11SamplerState> m_samplerState;
    };
} // namespace be::graphics::d3d11