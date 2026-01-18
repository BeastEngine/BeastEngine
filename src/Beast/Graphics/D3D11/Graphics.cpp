#include "Beast/Graphics/D3D11/Graphics.h"
#include "Beast/Graphics/D3D11/Utils.h"

#include "Beast/Graphics/Images.h"
#include "Beast/Graphics/Pipeline.h"

#include "Beast/Common/Limits.h"
#include "Beast/Common/TypeTraits.h"

namespace be::graphics::d3d11
{
    constexpr static decltype(auto) GetResource(const auto& container, const auto& resourceIndex)
    {
        return container.at(ToUnderlying(resourceIndex));
    }

    Graphics::Graphics(const Window& window)
        : m_api(CreateAPI(window.GetHandle())), m_renderTarget(d3d11::CreateRenderTarget(m_api))
    {
    }

    graphics::VertexBuffer Graphics::CreateVertexBuffer(uint32 stride, uint32 maxSize)
    {
        BE_ASSERT_ALWAYS(m_vertexBuffers.size() < MAX_VB_INDEX);

        const std::size_t index = m_vertexBuffers.size();
        m_vertexBuffers.emplace_back(d3d11::CreateVertexBuffer(m_api, stride, maxSize));

        return ToEnum<graphics::VertexBuffer>(index);
    }

    graphics::ConstantBuffer Graphics::CreateConstantBuffer(uint32 size)
    {
        BE_ASSERT_ALWAYS(m_constantBuffers.size() < MAX_CB_INDEX);

        const std::size_t index = m_constantBuffers.size();
        m_constantBuffers.emplace_back(d3d11::CreateConstantBuffer(m_api, size));

        return ToEnum<graphics::ConstantBuffer>(index);
    }

    graphics::VertexShader Graphics::CreateVertexShader(const fs::Path& filepath, const InputLayout& inputLayout)
    {
        BE_ASSERT_ALWAYS(m_vertexShaders.size() < MAX_VS_INDEX);

        const std::size_t index = m_vertexShaders.size();
        m_vertexShaders.emplace_back(d3d11::CreateVertexShader(m_api, filepath, inputLayout));

        return ToEnum<graphics::VertexShader>(index);
    }

    graphics::PixelShader Graphics::CreatePixelShader(const fs::Path& filepath)
    {
        BE_ASSERT_ALWAYS(m_pixelShaders.size() < MAX_PS_INDEX);

        const std::size_t index = m_pixelShaders.size();
        m_pixelShaders.emplace_back(d3d11::CreatePixelShader(m_api, filepath));

        return ToEnum<graphics::PixelShader>(index);
    }

    graphics::Texture Graphics::CreateTexture(const Image& textureData)
    {
        BE_ASSERT_ALWAYS(m_textures.size() < MAX_TEXTURE_INDEX);

        const std::size_t index = m_textures.size();
        m_textures.emplace_back(d3d11::CreateTexture(m_api, textureData));

        return ToEnum<graphics::Texture>(index);
    }

    void Graphics::UpdateVertexBuffer(graphics::VertexBuffer buffer, std::span<const Vertex> vertices)
    {
        GetResource(m_vertexBuffers, buffer).Update(m_api.Context(), vertices);
    }

    void Graphics::UpdateConstantBuffer(graphics::ConstantBuffer buffer, const void* const data)
    {
        GetResource(m_constantBuffers, buffer).Update(m_api.Context(), data);
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

            const auto& buffer = GetResource(m_constantBuffers, pipeline.constantBuffer);
            context.VSSetConstantBuffers(0, 1, buffer.Address());
        }

        // PSStage
        {
            const auto& shader = GetResource(m_pixelShaders, pipeline.pixelShaderStage.shader);
            context.PSSetShader(shader.Shader(), nullptr, 0);

            const auto& texture = GetResource(m_textures, pipeline.pixelShaderStage.texture);
            context.PSSetSamplers(0, 1, texture.SamplerAddress());
            context.PSSetShaderResources(0, 1, texture.ResourceViewAddress());
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
