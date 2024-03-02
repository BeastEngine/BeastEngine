#include "Beast/Graphics/D3D11/Graphics.h"
#include "Beast/Graphics/D3D11/Utils.h"

namespace be::graphics::d3d11
{
    constexpr static decltype(auto) GetResource(const auto& container, const auto& resourceRef)
    {
        return container.at(resourceRef.id.Raw());
    }

    Graphics::Graphics(const Window& window)
        : m_api(CreateAPI(window.GetHandle())), m_renderTarget(d3d11::CreateRenderTarget(m_api))
    {
    }

    graphics::VertexBuffer Graphics::CreateVertexBuffer(uint32 stride, uint32 maxSize)
    {
        m_vertexBuffers.emplace_back(d3d11::CreateVertexBuffer(m_api, stride, maxSize));
        Id id{m_vertexBuffers.size() - 1};

        return graphics::VertexBuffer{.id = id};
    }

    graphics::VertexShader Graphics::CreateVertexShader(const FilesystemPath& filepath, const InputLayout& inputLayout)
    {
        m_vertexShaders.emplace_back(d3d11::CreateVertexShader(m_api, filepath, inputLayout));
        Id id{m_vertexShaders.size() - 1};

        return graphics::VertexShader{.id = id};
    }

    graphics::PixelShader Graphics::CreatePixelShader(const FilesystemPath& filepath)
    {
        m_pixelShaders.emplace_back(d3d11::CreatePixelShader(m_api, filepath));
        Id id{m_pixelShaders.size() - 1};

        return graphics::PixelShader{.id = id};
    }

    void Graphics::UpdateVertexBuffer(graphics::VertexBuffer buffer, std::span<const Vertex> verticies)
    {
        m_vertexBuffers.at(buffer.id.Raw()).Update(m_api.Context(), verticies);
    }

    void Graphics::Draw(const Pipeline& pipeline)
    {
        auto& context = m_api.Context();

        // IAStage
        {
            const auto& buffer = GetResource(m_vertexBuffers, pipeline.vertexBuffer);
            const auto stride = buffer.Stride();
            const uint32 offset = 0;

            context.IASetVertexBuffers(0, 1, buffer.Address(), &stride, &offset);
        }

        // VSStage
        {
            const auto& shader = GetResource(m_vertexShaders, pipeline.vertexShaderStage.shader);
            context.IASetInputLayout(shader.Layout());
            context.VSSetShader(shader.Shader(), nullptr, 0u);
        }

        // PSStage
        {
            const auto& shader = GetResource(m_pixelShaders, pipeline.pixelShaderStage.shader);
            context.PSSetShader(shader.Shader(), nullptr, 0);
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
            context.RSSetViewports(1, &d3dViewport);
        }

        // Drawing
        {
            context.IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
            context.Draw(pipeline.drawCall.vertexCount, pipeline.drawCall.firstVertexIndex);
        }
    }

    void Graphics::Clear(const Color& color)
    {
        m_api.Context().OMSetRenderTargets(1, m_renderTarget.Address(), nullptr);
        m_api.Context().ClearRenderTargetView(m_renderTarget.View(), color.Data());
    }

    void Graphics::Present()
    {
        BE_DX_CALL(m_api.SwapChain().Present(1, 0));
    }
} // namespace be::graphics::d3d11
