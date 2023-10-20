#include "Renderer.h"

#include <Beast/Common/Utils/String.h>
#include <Beast/Common/Types.h>
#include <Beast/Ecs/View.h>

#include <spdlog/spdlog.h>

#include <algorithm>

Renderer::Renderer(be::Shared<be::graphics::IContext> context)
    : m_context(std::move(context))
{
    //[[maybe_unused]] const auto buffer = m_context->CreateVertexBuffer();
}

void Renderer::Run(const be::View<AccessList>& view)
{
    std::array<std::vector<const be::Sprite*>, be::LAYERS_COUNT> sprites{};
    for (const auto entity : view)
    {
        const auto& sprite = view.GetComponent<be::Sprite>(entity);
        sprites[be::ToUnderlying(sprite.layer)].push_back(&sprite);
    }

    const auto sort = [](std::vector<const be::Sprite*>& layerSprites) {
        std::sort(
            layerSprites.begin(),
            layerSprites.end(),
            [](auto&& first, auto&& second) {
                return first->texture.id < second->texture.id;
            }
        );
    };

    std::for_each(sprites.begin(), sprites.end(), sort);
    for (std::vector<const be::Sprite*>& layerSprites : sprites)
    {
        spdlog::info("Layer");
        for (const be::Sprite* sprite : layerSprites)
        {
            spdlog::info(
                "Sprite: {{ Texture: {{ Id: {}, UV: {{ {}, {} }} }}, Material: {{ Color: {{{}, {}, {}, {}}} }} }}",
                ToString(sprite->texture.id),
                sprite->texture.uvCoords.x,
                sprite->texture.uvCoords.y,
                sprite->material.color.r,
                sprite->material.color.g,
                sprite->material.color.b,
                sprite->material.color.a
            );
        }
    }

    m_context->Clear({0.6f, 0.2f, 0.3f, 1.0f});
    m_context->Present();

    /**
         * Alternative.
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
