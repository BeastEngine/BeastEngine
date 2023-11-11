#pragma once
#include "Beast/Graphics/D3D11/D3D11.h"
#include "Beast/Graphics/Types.h"
#include "Beast/Common/Types.h"

#include "Beast/Debugging.h"

#include <vector>

namespace be::graphics::d3d11
{
    namespace wrl = Microsoft::WRL;

    class PixelShader final
    {
    public:
        PixelShader(ID3D11Device& device, ID3DBlob& shaderByteCode)
        {
            CheckResult(device.CreatePixelShader(shaderByteCode.GetBufferPointer(), shaderByteCode.GetBufferSize(), nullptr, &m_shader));
        }

        void Use(ID3D11DeviceContext& context)
        {
            context.PSSetShader(m_shader.Get(), nullptr, 0);
        }

    private:
        wrl::ComPtr<ID3D11PixelShader> m_shader;
    };
} // namespace be::graphics::d3d11
