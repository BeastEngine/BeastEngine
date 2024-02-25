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
        VertexBuffer(wrl::ComPtr<ID3D11Buffer> buffer, uint32 stride, uint32 size)
            : m_bufferPtr(std::move(buffer)), m_stride(stride), m_size(size)
        {
        }

        void Update(ID3D11DeviceContext& context, std::span<const Vertex> data) const
        {
            const auto dataSize = data.size() * m_stride;
            BE_ASSERT_MSG(dataSize <= m_size, "Given data is bigger than the buffer's size! Data size: {} | Buffer size: {}", dataSize, m_size);

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
        wrl::ComPtr<ID3D11Buffer> m_bufferPtr;
        uint32 m_stride = sizeof(Vertex);
        uint32 m_size = 0;
    };
}
