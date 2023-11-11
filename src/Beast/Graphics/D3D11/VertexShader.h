#pragma once
#include "Beast/Graphics/D3D11/D3D11.h"
#include "Beast/Graphics/Types.h"
#include "Beast/Common/Types.h"

#include "Beast/Debugging.h"

#include <vector>

namespace be::graphics::d3d11
{
    namespace wrl = Microsoft::WRL;

    static constexpr DXGI_FORMAT ConvertInputElementFormat(InputLayout::VertexAttribute::Format format)
    {
        switch (format)
        {
        case be::graphics::InputLayout::VertexAttribute::Format::Float4:
            return DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
        case be::graphics::InputLayout::VertexAttribute::Format::Float2:
            return DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT;
        default:
            BE_DEBUG_LOG_WARNING("Given attribute {} is not recognized a supported d3d11 dxgi format.", ToUnderlying(format));
            return DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
        }
    }

    class VertexShader final
    {
    public:
        VertexShader(ID3D11Device& device, ID3DBlob& shaderByteCode, const InputLayout& inputLayout)
        {
            CheckResult(device.CreateVertexShader(shaderByteCode.GetBufferPointer(), shaderByteCode.GetBufferSize(), nullptr, &m_shader));

            std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements;
            inputElements.reserve(inputLayout.vertexAttributes.size());
            for (const auto& attribute : inputLayout.vertexAttributes)
            {
                inputElements.emplace_back(D3D11_INPUT_ELEMENT_DESC{
                    .SemanticName = attribute.name,
                    .SemanticIndex = attribute.index,
                    .Format = ConvertInputElementFormat(attribute.format),
                    .InputSlot = 0u,
                    .AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT,
                    .InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA,
                    .InstanceDataStepRate = 0u,
                });
            }

            CheckResult(device.CreateInputLayout(inputElements.data(), static_cast<UINT>(inputElements.size()), shaderByteCode.GetBufferPointer(), shaderByteCode.GetBufferSize(), &m_inputLayout));
        }

        void Use(ID3D11DeviceContext& context)
        {
            context.IASetInputLayout(m_inputLayout.Get());
            context.VSSetShader(m_shader.Get(), nullptr, 0u);
        }

    private:
        wrl::ComPtr<ID3D11VertexShader> m_shader;
        wrl::ComPtr<ID3D11InputLayout> m_inputLayout;
    };
} // namespace be::graphics::d3d11
