#include "Beast/Graphics/2DRenderer.h"
#include "Beast/Graphics/Camera2D.h"
#include "Beast/Graphics/Pipeline.h"

#include "Beast/Debug.h"

// TEMP
#include "Beast/Graphics/ImageLoader.h"

namespace be::graphics
{
    static constexpr float PRIMITIVE_SCALE = 50.0f;

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

        m_cameraCBuffer = m_graphics->CreateConstantBuffer(sizeof(Mat4));

        // TEMP - only works if running from IDE
        const Result<Image> imageResult = LoadImageFromFile("data/textures/t.png");
        BE_ASSERT(imageResult);
        m_texture = m_graphics->CreateTexture(imageResult.Value());
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

    void Renderer2D::AddSprite(const Vec2& position, const Sprite& sprite)
    {
        // TODO: Let's try render the texture!

        BE_ASSERT_MSG(m_hasFrameStarted, "Frame must be started before sprites can be added!");
        m_framePrimitives.emplace_back(Primitive{.position = position * PRIMITIVE_SCALE, .color = sprite.color});
    }

    void Renderer2D::EndFrame(const Camera2D& camera)
    {
        static constexpr float SCALE_HALF = PRIMITIVE_SCALE / 2.0f;
        BE_ASSERT_MSG(m_hasFrameStarted, "Frame must be started first, before it can be ended!");
        {
            const uint32 verticesCount = static_cast<uint32>(m_framePrimitives.size() * 6u);

            std::vector<Vertex> vertices;
            vertices.reserve(verticesCount);

            for (const auto& primitive : m_framePrimitives)
            {
                vertices.emplace_back(Vertex{
                    .position = Vec2{-SCALE_HALF, -SCALE_HALF} + primitive.position,
                    .color = primitive.color,
                    .uv = Vec2{0.0f, 0.0f},
                });
                vertices.emplace_back(Vertex{
                    .position = Vec2{-SCALE_HALF, SCALE_HALF} + primitive.position,
                    .color = primitive.color,
                    .uv = Vec2{0.0f, 1.0f},
                });
                vertices.emplace_back(Vertex{
                    .position = Vec2{SCALE_HALF, SCALE_HALF} + primitive.position,
                    .color = primitive.color,
                    .uv = Vec2{1.0f, 1.0f},
                });
                vertices.emplace_back(Vertex{
                    .position = Vec2{-SCALE_HALF, -SCALE_HALF} + primitive.position,
                    .color = primitive.color,
                    .uv = Vec2{0.0f, 0.0f},
                });
                vertices.emplace_back(Vertex{
                    .position = Vec2{SCALE_HALF, SCALE_HALF} + primitive.position,
                    .color = primitive.color,
                    .uv = Vec2{1.0f, 1.0f},
                });
                vertices.emplace_back(Vertex{
                    .position = Vec2{SCALE_HALF, -SCALE_HALF} + primitive.position,
                    .color = primitive.color,
                    .uv = Vec2{1.0f, 0.0f},
                });
            }

            be::Mat4 viewMatrix = camera.GetViewMatrix();
            auto& translationVec = viewMatrix[3];
            translationVec *= glm::vec4{PRIMITIVE_SCALE, PRIMITIVE_SCALE, 1.0f, 1.0f};

            m_graphics->UpdateVertexBuffer(m_buffer, vertices);
            m_graphics->UpdateConstantBuffer(m_cameraCBuffer, &viewMatrix[0]);

            Pipeline pipeline{};
            pipeline.vertexBuffer = m_buffer;
            pipeline.vertexShaderStage.shader = m_vertexShader;
            pipeline.pixelShaderStage.shader = m_pixelShader;
            pipeline.pixelShaderStage.texture = m_texture;
            pipeline.viewport = camera.GetViewport();
            //pipeline.viewport.dimensions = {1520, 825};
            pipeline.drawCall.vertexCount = verticesCount;
            pipeline.constantBuffer = m_cameraCBuffer;

            m_graphics->Clear({0.35f, 0.35f, 0.35f, 1.0f});
            m_graphics->Draw(pipeline);
            m_graphics->Present();
        }
        BE_DEBUG_EXPRESSION(m_hasFrameStarted = false);
        BE_DEBUG_EXPRESSION(m_hasFrameEnded = true);
    }
} // namespace be::graphics
