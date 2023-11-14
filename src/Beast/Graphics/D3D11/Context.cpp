#include "Beast/Graphics/D3D11/Context.h"
#include "Beast/Graphics/D3D11/Asserts.h"

namespace be::graphics::d3d11
{
    Context::Context(Device& device)
        : m_device(device)
    {
        m_context = m_device.GetContext();
        m_swapChain = m_device.GetSwapChain();
        m_renderTargetView = m_device.CreateRenderTargetView(*m_swapChain.Get());
    }

    void Context::Draw(const Pipeline& pipeline) const noexcept
    {
        // IAStage
        {
            const auto& buffer = m_device.GetBuffer(pipeline.vertexBuffer);
            const auto stride = buffer.Stride();
            const uint32 offset = 0;

            // TODO: Make sure the &buffer.Stride() isn't risky!
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

    void graphics::d3d11::Context::Clear(const Color& color) const noexcept
    {
        m_context->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), nullptr);
        m_context->ClearRenderTargetView(m_renderTargetView.Get(), color.Data());
    }

    void graphics::d3d11::Context::Present() const noexcept
    {
        Assert(m_swapChain->Present(1, 0));
    }

    void Context::UpdateVertexBuffer(graphics::VertexBuffer buffer, std::span<const Vertex> verticies)
    {
        m_device.GetBuffer(buffer).Update(*m_context.Get(), verticies);
    }
} // namespace be::graphics::d3d11
