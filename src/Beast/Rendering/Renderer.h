#pragma once
#include <Beast/Ecs/AccessList.h>
#include <Beast/Ecs/Components/Sprite.h>

#include <Beast/Graphics/IContext.h>
#include <Beast/Common/Types.h>

namespace be
{
    template<typename>
    class View;

    class Renderer final
    {
    public:
        struct AccessList : be::BaseAccessList
        {
            using Get = Components<Sprite>;
        };

        explicit Renderer(Shared<graphics::IContext> context);
        void Run(const View<AccessList>& view);

    private:
        struct RenderingResource
        {
            Texture texture;
            Material material;
        };

        std::vector<RenderingResource> m_resources;
        Shared<graphics::IContext> m_context;
    };
}
