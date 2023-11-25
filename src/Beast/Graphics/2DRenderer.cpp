#include "Beast/Graphics/2DRenderer.h"

#include "Beast/Debug.h"

namespace be::graphics
{
    Renderer2D::Renderer2D(be::Unique<IGraphics> graphics)
        : m_graphics(std::move(graphics))
    {
        m_buffer = m_graphics->CreateVertexBuffer(sizeof(Vertex), sizeof(Vertex) * 3 * 1000);
        be::graphics::InputLayout layout{
            .vertexAttributes = {
                {"POSITION", be::graphics::InputLayout::VertexAttribute::Format::Vec2},
                {"COLOR", be::graphics::InputLayout::VertexAttribute::Format::Vec4},
                {"TEXCOORDS", be::graphics::InputLayout::VertexAttribute::Format::Vec2},
            },
        };

        m_vertexShader = m_graphics->CreateVertexShader("VertexShader.csoo", layout);
        m_pixelShader = m_graphics->CreatePixelShader("PixelShader.cso");
    }

    void Renderer2D::StartFrame()
    {
        // TODO: I think I can replace it with BE_ASSERT_MSG(!m_hasFrameStarted, ...); but I have to double check!
        BE_ASSERT_MSG(m_hasFrameEnded, "Previous frame must be ended before a new one can be started!");
        {
            m_framePrimitives.clear();
        }
        BE_DEBUG_EXPRESSION(m_hasFrameStarted = true);
        BE_DEBUG_EXPRESSION(m_hasFrameEnded = false);
    }

    void Renderer2D::AddSprite(const Transform& transform, const Sprite& sprite)
    {
        BE_ASSERT_MSG(m_hasFrameStarted, "Frame must be started before sprites can be added!");

        Primitive primitive{.transform = transform, .texture = sprite.texture, .material = sprite.material};
        m_framePrimitives.emplace_back(std::move(primitive));
    }

    void Renderer2D::EndFrame()
    {
        BE_ASSERT_MSG(m_hasFrameStarted, "Frame must be started before it can be ended!");
        {
            const uint32 verticesCount = static_cast<uint32>(m_framePrimitives.size() * 3u);

            const auto& ctx = m_graphics->GetContext();
            std::vector<Vertex> vertices;
            vertices.reserve(verticesCount);

            for (const auto& primitive : m_framePrimitives)
            {
                vertices.emplace_back(Vertex{
                    .position = Vec2{-0.1f, -0.1f} + primitive.transform.position,
                    .color = primitive.material.color,
                });
                vertices.emplace_back(Vertex{
                    .position = Vec2{0.0f, 0.1f} + primitive.transform.position,
                    .color = primitive.material.color,
                });
                vertices.emplace_back(Vertex{
                    .position = Vec2{0.1f, -0.1f} + primitive.transform.position,
                    .color = primitive.material.color,
                });
            }
            ctx.UpdateVertexBuffer(m_buffer, vertices);

            Pipeline pipeline{};
            pipeline.vertexBuffer = m_buffer;
            pipeline.vertexShaderStage.shader = m_vertexShader;
            pipeline.pixelShaderStage.shader = m_pixelShader;
            pipeline.viewport.dimensions = {800.0f, 600.0f};
            pipeline.drawCall.vertexCount = verticesCount;

            //pipeline.OMStage.renderTarget;
            //= // renderTarget;

            ctx.Clear({0.6f, 0.2f, 0.3f, 1.0f});
            ctx.Draw(pipeline);
            ctx.Present();
        }
        BE_DEBUG_EXPRESSION(m_hasFrameStarted = false);
        BE_DEBUG_EXPRESSION(m_hasFrameEnded = true);
    }
} // namespace be::graphics
