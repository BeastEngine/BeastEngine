#include "Renderer.h"

#include <Beast/Common/Utils/String.h>
#include <Beast/Common/Types.h>
#include <Beast/Ecs/View.h>

#include <spdlog/spdlog.h>

#include <algorithm>

Renderer::Renderer(be::Shared<be::graphics::Graphics> graphics)
    : m_graphics(std::move(graphics))
{
    [[maybe_unused]] const auto buffer = m_graphics->Device().CreateVertexBuffer(0u, 96u);
    be::graphics::InputLayout layout{
        .vertexAttributes = {
            {"POSITION", be::graphics::InputLayout::VertexAttribute::Format::Vec2},
            {"COLOR", be::graphics::InputLayout::VertexAttribute::Format::Vec4},
            {"TEXCOORDS", be::graphics::InputLayout::VertexAttribute::Format::Vec2},
        },
    };

    [[maybe_unused]] const auto vertexShader = m_graphics->Device().CreateVertexShader("VertexShader.cso", layout);
    [[maybe_unused]] const auto pixelShader = m_graphics->Device().CreatePixelShader("PixelShader.cso");
}

void Renderer::Run(const be::View<AccessList>&)
{
    //std::array<std::vector<const be::Sprite*>, be::LAYERS_COUNT> sprites{};
    //for (const auto entity : view)
    //{
    //    const auto& sprite = view.GetComponent<be::Sprite>(entity);
    //    sprites[be::ToUnderlying(sprite.layer)].push_back(&sprite);

    //    const auto& transform = view.GetComponent<be::Transform>(entity);
    //    spdlog::info("Position: {{ {}, {} }}", transform.position.x, transform.position.y);
    //}

    //const auto sort = [](std::vector<const be::Sprite*>& layerSprites) {
    //    std::sort(
    //        layerSprites.begin(),
    //        layerSprites.end(),
    //        [](auto&& first, auto&& second) {
    //            return first->texture.id < second->texture.id;
    //        }
    //    );
    //};

    //std::for_each(sprites.begin(), sprites.end(), sort);
    //for (std::vector<const be::Sprite*>& layerSprites : sprites)
    //{
    //    //spdlog::info("Layer");
    //    for (const be::Sprite* sprite : layerSprites)
    //    {
    //        spdlog::info("{}", be::ToUnderlying(sprite->layer));
    //    }
    //}

    m_graphics->Context().Clear({0.6f, 0.2f, 0.3f, 1.0f});
    //m_graphics->Context().Clear({0.0f, 0.0f, 0.0f, 1.0f});
    m_graphics->Device().Run();
    m_graphics->Context().Present();

    /**
         * Alternative.
         */
    /**
     * So, I want to be able to do this:
     * for (const auto entity : view)
     * {
     *      const auto& sprite = view.GetComponent<be::Sprite>(entity);
            const auto& transform = view.GetComponent<be::Transform>(entity);

            m_renderer->RenderSprite(transform, sprite);
     * }
     * 
     * And then later in the game loop:
     * m_renderer->Render();
     */
    /*auto renderPass = m_renderer->CreatePass();
        for (const auto entity : view)
        {
            const auto& sprite = view.GetComponent<Sprite>(entity);
            renderPass.AddSprite(sprite, view.GetComponent<Transform>(entity);
        }
        // And this, internally, does all the sorting, contains the pipeline etc.
        m_renderer->SubmitPass(std::move(renderPass));
        */
}
