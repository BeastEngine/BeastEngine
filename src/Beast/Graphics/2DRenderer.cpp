#include "Beast/Graphics/2DRenderer.h"
#include "Beast/Graphics/Camera2D.h"
#include "Beast/Graphics/Pipeline.h"
#include "Beast/Graphics/IGraphics.h"

#include "Beast/Debug.h"
#include "Beast/Common/Filesystem/ResourcesManager.h"

// TEMP
#include "Beast/Graphics/ImageLoader.h"

namespace be::graphics
{
    static constexpr float PRIMITIVE_SCALE = 50.0f;

    Renderer2D::Renderer2D(be::Unique<IGraphics> graphics, uint32 maxNumberOfSprites, be::Unique<fs::ResourcesManager> resourcesManager)
        : m_graphics(std::move(graphics)), m_resourcesManager(std::move(resourcesManager))
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

        // TODO: These should also come from the resources manager
        m_vertexShader = m_graphics->CreateVertexShader("VertexShader.cso", layout);
        m_pixelShader = m_graphics->CreatePixelShader("PixelShader.cso");

        m_cameraCBuffer = m_graphics->CreateConstantBuffer(sizeof(Mat4));
    }

    // Declared here to be able to use Unique with incomplete type
    Renderer2D::~Renderer2D() = default;

    void Renderer2D::StartFrame()
    {
        BE_DEBUG_EXPRESSION(BE_ASSERT_MSG(m_hasFrameStarted == false, "Previous frame must be ended before a new one can be started!"));
        {
            m_framePrimitives.clear();
        }
        BE_DEBUG_EXPRESSION(m_hasFrameStarted = true);
    }

    void Renderer2D::AddSprite(const Vec2& position, const Sprite& sprite)
    {
        BE_DEBUG_EXPRESSION(BE_ASSERT_MSG(m_hasFrameStarted, "Frame must be started before sprites can be added!"));
        m_framePrimitives[sprite.texture].emplace_back(Primitive{.position = position * PRIMITIVE_SCALE, .sprite = sprite});
    }

    void Renderer2D::EndFrame(const Camera2D& camera)
    {
        static constexpr float SCALE_HALF = PRIMITIVE_SCALE / 2.0f;
        BE_DEBUG_EXPRESSION(BE_ASSERT_MSG(m_hasFrameStarted, "Frame must be started first, before it can be ended!"));
        {
            be::Mat4 viewMatrix = camera.GetViewMatrix();
            auto& translationVec = viewMatrix[3];
            translationVec *= glm::vec4{PRIMITIVE_SCALE, PRIMITIVE_SCALE, 1.0f, 1.0f};

            m_graphics->UpdateConstantBuffer(m_cameraCBuffer, &viewMatrix[0]);

            m_graphics->Clear({0.35f, 0.35f, 0.35f, 1.0f});

            std::vector<Vertex> vertices;

            for (const auto& [texture, primitives] : m_framePrimitives)
            {
                const uint32 verticesCount = static_cast<uint32>(primitives.size() * 6u);
                vertices.clear();
                vertices.reserve(verticesCount);

                for (const auto& primitive : primitives)
                {
                    vertices.emplace_back(Vertex{
                        .position = Vec2{-SCALE_HALF, -SCALE_HALF} + primitive.position,
                        .color = primitive.sprite.color,
                        .uv = Vec2{0.0f, 0.0f},
                    });
                    vertices.emplace_back(Vertex{
                        .position = Vec2{-SCALE_HALF, SCALE_HALF} + primitive.position,
                        .color = primitive.sprite.color,
                        .uv = Vec2{0.0f, 1.0f},
                    });
                    vertices.emplace_back(Vertex{
                        .position = Vec2{SCALE_HALF, SCALE_HALF} + primitive.position,
                        .color = primitive.sprite.color,
                        .uv = Vec2{1.0f, 1.0f},
                    });
                    vertices.emplace_back(Vertex{
                        .position = Vec2{-SCALE_HALF, -SCALE_HALF} + primitive.position,
                        .color = primitive.sprite.color,
                        .uv = Vec2{0.0f, 0.0f},
                    });
                    vertices.emplace_back(Vertex{
                        .position = Vec2{SCALE_HALF, SCALE_HALF} + primitive.position,
                        .color = primitive.sprite.color,
                        .uv = Vec2{1.0f, 1.0f},
                    });
                    vertices.emplace_back(Vertex{
                        .position = Vec2{SCALE_HALF, -SCALE_HALF} + primitive.position,
                        .color = primitive.sprite.color,
                        .uv = Vec2{1.0f, 0.0f},
                    });
                }

                m_graphics->UpdateVertexBuffer(m_buffer, vertices);

                Pipeline pipeline{};
                pipeline.vertexBuffer = m_buffer;
                pipeline.vertexShaderStage.shader = m_vertexShader;
                pipeline.pixelShaderStage.shader = m_pixelShader;
                pipeline.pixelShaderStage.texture = GetOrCreateTexture(texture);
                pipeline.viewport = camera.GetViewport();
                //pipeline.viewport.dimensions = {1520, 825};
                pipeline.drawCall.vertexCount = verticesCount;
                pipeline.constantBuffer = m_cameraCBuffer;

                m_graphics->Draw(pipeline);
            }
            m_graphics->Present();
        }
        BE_DEBUG_EXPRESSION(m_hasFrameStarted = false);
    }

    Texture Renderer2D::GetOrCreateTexture(TextureId id)
    {
        const auto foundTexture = m_textures.find(id);
        if (foundTexture != m_textures.end())
        {
            return foundTexture->second;
        }

        const RefResult<fs::Path> texturePath = m_resourcesManager->GetResourcePath(id);
        const Result<Image> imageResult = LoadImageFromFile(texturePath.Value());
        BE_ASSERT(imageResult);

        Texture& newTexture = m_textures[id];
        newTexture = m_graphics->CreateTexture(imageResult.Value());

        return newTexture;
    }
} // namespace be::graphics
