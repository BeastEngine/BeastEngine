#include "Beast/Graphics/2DRenderer.hpp"

#include "Beast/Debug.hpp"

namespace be::graphics
{
    Renderer2D::Renderer2D(be::Unique<IGraphics> graphics, uint32 maxNumberOfSprites)
        : m_graphics(std::move(graphics))
    {
        const uint32 vertexCount = sizeof(Vertex) * 6 * maxNumberOfSprites;
        m_buffer = m_graphics->CreateVertexBuffer(VERTEX_STRIDE, vertexCount);

        be::graphics::InputLayout layout{
            .vertexAttributes = {
                {"POSITION", be::graphics::InputLayout::VertexAttribute::Format::Vec2},
                {"COLOR", be::graphics::InputLayout::VertexAttribute::Format::Vec4},
                {"TEXCOORDS", be::graphics::InputLayout::VertexAttribute::Format::Vec2},
            },
        };

        m_vertexShader = m_graphics->CreateVertexShader("VertexShader.cso", layout);
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

    void Renderer2D::AddSprite(const Vec2& position, const Color& color)
    {
        BE_ASSERT_MSG(m_hasFrameStarted, "Frame must be started before sprites can be added!");

        Primitive primitive{.position = position, .color = color};
        m_framePrimitives.emplace_back(std::move(primitive));
    }

    void Renderer2D::EndFrame(const Viewport& viewport)
    {
        BE_ASSERT_MSG(m_hasFrameStarted, "Frame must be started first, before it can be ended!");
        {
            const uint32 verticesCount = static_cast<uint32>(m_framePrimitives.size() * 6u);

            std::vector<Vertex> vertices;
            vertices.reserve(verticesCount);

            for (const auto& primitive : m_framePrimitives)
            {
                vertices.emplace_back(Vertex{
                    .position = Vec2{-0.1f, -0.1f} + primitive.position,
                    .color = primitive.color,
                });
                vertices.emplace_back(Vertex{
                    .position = Vec2{-0.1f, 0.1f} + primitive.position,
                    .color = primitive.color,
                });
                vertices.emplace_back(Vertex{
                    .position = Vec2{0.1f, 0.1f} + primitive.position,
                    .color = primitive.color,
                });
                vertices.emplace_back(Vertex{
                    .position = Vec2{-0.1f, -0.1f} + primitive.position,
                    .color = primitive.color,
                });
                vertices.emplace_back(Vertex{
                    .position = Vec2{0.1f, 0.1f} + primitive.position,
                    .color = primitive.color,
                });
                vertices.emplace_back(Vertex{
                    .position = Vec2{0.1f, -0.1f} + primitive.position,
                    .color = primitive.color,
                });
            }
            m_graphics->UpdateVertexBuffer(m_buffer, vertices);

            Pipeline pipeline{};
            pipeline.vertexBuffer = m_buffer;
            pipeline.vertexShaderStage.shader = m_vertexShader;
            pipeline.pixelShaderStage.shader = m_pixelShader;
            pipeline.viewport = viewport;
            pipeline.drawCall.vertexCount = verticesCount;

            m_graphics->Clear({0.35f, 0.35f, 0.35f, 1.0f});
            m_graphics->Draw(pipeline);
            m_graphics->Present();
        }
        BE_DEBUG_EXPRESSION(m_hasFrameStarted = false);
        BE_DEBUG_EXPRESSION(m_hasFrameEnded = true);
    }
} // namespace be::graphics
