#include <Beast/Rendering/Renderer.h>
#include <Beast/Common/Utils/String.h>
#include <Beast/Common/Types.h>
#include <Beast/Ecs/View.h>

#include <spdlog/spdlog.h>

#include <algorithm>

namespace be
{
    Renderer::Renderer(Shared<graphics::IContext> context)
        : m_context(std::move(context))
    {
    }

    void Renderer::Run(const View<AccessList>& view)
    {
        std::array<std::vector<const Sprite*>, LAYERS_COUNT> sprites{};
        for (const auto entity : view)
        {
            const auto& sprite = view.GetComponent<Sprite>(entity);
            sprites[ToUnderlying(sprite.layer)].push_back(&sprite);
        }

        const auto sort = [](std::vector<const Sprite*>& layerSprites) {
            std::sort(
                layerSprites.begin(),
                layerSprites.end(),
                [](auto&& first, auto&& second) {
                    return first->texture.id < second->texture.id;
                }
            );
        };

        std::for_each(sprites.begin(), sprites.end(), sort);
        for (std::vector<const Sprite*>& layerSprites : sprites)
        {
            spdlog::info("Layer");
            for (const Sprite* sprite : layerSprites)
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

        m_context->ClearRenderTargetView({0.6f, 0.2f, 0.3f, 1.0f});
        m_context->Present();
    }
} // namespace be
