#include "Beast/Graphics/D3D11/Context.h"
#include "Beast/Graphics/D3D11/Debug.h"
#include "Beast/Graphics/D3D11/Device.h"

namespace be::graphics::d3d11
{
    Context::Context(Device& device, IDXGISwapChain* swapChain, ID3D11DeviceContext* context, wrl::ComPtr<ID3D11RenderTargetView> renderTargetView)
        : m_device(device), m_swapChain(swapChain), m_context(context), m_renderTargetView(std::move(renderTargetView))
    {
    }

    void Context::Draw(const Pipeline& pipeline) const
    {
        // IAStage
        {
            const auto& buffer = m_device.GetBuffer(pipeline.vertexBuffer);
            const auto stride = buffer.Stride();
            const uint32 offset = 0;

            m_context->IASetVertexBuffers(0, 1, buffer.BufferAddress(), &stride, &offset);
        }

        // VSStage
        {
            const auto& shader = m_device.GetShader(pipeline.vertexShaderStage.shader);
            m_context->IASetInputLayout(shader.GetLayout());
            m_context->VSSetShader(shader.GetShader(), nullptr, 0u);
        }

        // PSStage
        {
            const auto& shader = m_device.GetShader(pipeline.pixelShaderStage.shader);
            m_context->PSSetShader(shader.GetShader(), nullptr, 0);
        }

        // RSStage
        {
            D3D11_VIEWPORT d3dViewport{
                .TopLeftX = 0.0f,
                .TopLeftY = 0.0f,
                .Width = pipeline.viewport.dimensions.x,
                .Height = pipeline.viewport.dimensions.y,
                .MinDepth = 0.0f,
                .MaxDepth = 1.0f,
            };
            m_context->RSSetViewports(1, &d3dViewport);
        }

        // Drawing
        {
            m_context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
            m_context->Draw(pipeline.drawCall.vertexCount, pipeline.drawCall.firstVertexIndex);
        }
    }

    void graphics::d3d11::Context::Clear(const Color& color) const
    {
        m_context->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), nullptr);
        m_context->ClearRenderTargetView(m_renderTargetView.Get(), color.Data());
    }

    void graphics::d3d11::Context::Present() const
    {
        BE_DX_CALL(m_swapChain->Present(1, 0));
    }

    void Context::UpdateVertexBuffer(graphics::VertexBuffer buffer, std::span<const Vertex> verticies) const
    {
        m_device.GetBuffer(buffer).Update(*m_context, verticies);
    }
} // namespace be::graphics::d3d11
