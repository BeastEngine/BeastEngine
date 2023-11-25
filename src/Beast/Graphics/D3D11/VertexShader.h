#pragma once
#include "Beast/Graphics/D3D11/D3D11.h"
#include "Beast/Graphics/Types.h"
#include "Beast/Common/Types.h"

#include "Beast/Debug.h"

#include <vector>

namespace be::graphics::d3d11
{
    namespace wrl = Microsoft::WRL;

    static constexpr DXGI_FORMAT ConvertInputElementFormat(InputLayout::VertexAttribute::Format format)
    {
        switch (format)
        {
        case be::graphics::InputLayout::VertexAttribute::Format::Vec4:
            return DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
        case be::graphics::InputLayout::VertexAttribute::Format::Vec2:
            return DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT;
        default:
            BE_DEBUG_LOG_WARNING("Given attribute {} is not recognized a supported d3d11 dxgi format.", ToUnderlying(format));
            return DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
        }
    }

    class VertexShader final
    {
    public:
        VertexShader(wrl::ComPtr<ID3D11VertexShader> shader, wrl::ComPtr<ID3D11InputLayout> inputLayout)
            : m_shader(std::move(shader)), m_inputLayout(std::move(inputLayout))
        {
        }

        /*void Bind(ID3D11DeviceContext& context)
        {
            context.IASetInputLayout(m_inputLayout.Get());
            context.VSSetShader(m_shader.Get(), nullptr, 0u);
        }*/

        ID3D11VertexShader* GetShader() const
        {
            return m_shader.Get();
        }

        ID3D11InputLayout* GetLayout() const
        {
            return m_inputLayout.Get();
        }

    private:
        wrl::ComPtr<ID3D11VertexShader> m_shader;
        wrl::ComPtr<ID3D11InputLayout> m_inputLayout;
    };
} // namespace be::graphics::d3d11
