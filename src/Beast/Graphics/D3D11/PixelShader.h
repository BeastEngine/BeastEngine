#pragma once
#include "Beast/Graphics/D3D11/D3D11.h"
#include "Beast/Graphics/Types.h"
#include "Beast/Common/Types.h"

#include "Beast/Debug.h"

#include <vector>

namespace be::graphics::d3d11
{
    namespace wrl = Microsoft::WRL;

    class PixelShader final
    {
    public:
        PixelShader(wrl::ComPtr<ID3D11PixelShader> shader)
            : m_shader(std::move(shader))
        {
        }

        /*void Bind(ID3D11DeviceContext& context)
        {
            context.PSSetShader(m_shader.Get(), nullptr, 0);
        }*/

        ID3D11PixelShader* GetShader() const
        {
            return m_shader.Get();
        }

    private:
        wrl::ComPtr<ID3D11PixelShader> m_shader;
    };
} // namespace be::graphics::d3d11
