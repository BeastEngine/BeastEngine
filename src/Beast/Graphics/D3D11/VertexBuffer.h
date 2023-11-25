#pragma once
#include "Beast/Graphics/D3D11/D3D11.h"
#include "Beast/Graphics/D3D11/Debug.h"
#include "Beast/Graphics/Types.h"

#include "Beast/Common/Types.h"

namespace be::graphics::d3d11
{
    namespace wrl = Microsoft::WRL;

    class VertexBuffer final
    {
    public:
        VertexBuffer(wrl::ComPtr<ID3D11Buffer> buffer, uint32 stride)
            : m_bufferPtr(std::move(buffer)), m_stride(stride)
        {
        }

        /*void Bind(ID3D11DeviceContext& context, UINT inputSlot = 0) const
        {
            context.IASetVertexBuffers(inputSlot, 1, m_bufferPtr.GetAddressOf(), &m_stride, &m_offset);
        }*/

        void Update(ID3D11DeviceContext& context, std::span<const Vertex> data) const
        {
            D3D11_MAPPED_SUBRESOURCE subresource;
            BE_DX_CALL(context.Map(m_bufferPtr.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subresource));
            {
                memcpy(subresource.pData, data.data(), data.size() * m_stride);
            }
            context.Unmap(m_bufferPtr.Get(), 0);
        }

        const uint32 Stride() const
        {
            return m_stride;
        }

        ID3D11Buffer* const* BufferAddress() const
        {
            return m_bufferPtr.GetAddressOf();
        }

    private:
        uint32 m_stride = sizeof(Vertex);
        wrl::ComPtr<ID3D11Buffer> m_bufferPtr;
    };
}
