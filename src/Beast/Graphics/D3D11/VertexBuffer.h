#pragma once
#include "Beast/Graphics/D3D11/D3D11.h"
#include "Beast/Graphics/D3D11/Asserts.h"
#include "Beast/Graphics/Components.h"

#include "Beast/Common/Types.h"

namespace be::graphics::d3d11
{
    namespace wrl = Microsoft::WRL;

    class VertexBuffer
    {
    public:
        VertexBuffer(ID3D11Device& device)
        {
            D3D11_BUFFER_DESC bufferDescriptor = {};
            bufferDescriptor.ByteWidth = sizeof(Vertex); // This must be the MAX size of the buffer
            bufferDescriptor.Usage = D3D11_USAGE_DYNAMIC;
            bufferDescriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            bufferDescriptor.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
            bufferDescriptor.MiscFlags = 0;
            bufferDescriptor.StructureByteStride = m_stride;

            CheckResult(device.CreateBuffer(&bufferDescriptor, nullptr, &m_bufferPtr));
        }

        void Bind(ID3D11DeviceContext& context, UINT inputSlot = 0) const
        {
            context.IASetVertexBuffers(inputSlot, 1, m_bufferPtr.GetAddressOf(), &m_stride, &m_offset);
        }

        void Update(ID3D11DeviceContext& context, std::span<const Vertex> data)
        {
            D3D11_MAPPED_SUBRESOURCE subresource;
            CheckResult(context.Map(m_bufferPtr.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subresource));
            {
                memcpy(subresource.pData, data.data(), data.size() * m_stride);
            }
            context.Unmap(m_bufferPtr.Get(), 0);
        }

    private:
        UINT m_stride = sizeof(Vertex);
        UINT m_offset = 0;

        wrl::ComPtr<ID3D11Buffer> m_bufferPtr;
    };
}
