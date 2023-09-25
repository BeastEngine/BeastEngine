#pragma once
#include <Beast/Graphics/Rendering/Components/Sprite.h>
#include <Beast/Graphics/IContext.h>

#include <Beast/Ecs/AccessList.h>
#include <Beast/Common/Types.h>

namespace be
{
    template<typename>
    class View;
}

namespace be::graphics
{
    class Renderer final
    {
    public:
        struct AccessList : be::BaseAccessList
        {
            using Get = Components<Sprite>;
        };

        explicit Renderer(Shared<IContext> context);
        void Run(const View<AccessList>& view);

    private:
        struct RenderingResource
        {
            Texture texture;
            Material material;
        };

        std::vector<RenderingResource> m_resources;
        Shared<IContext> m_context;
    };
} // namespace be::graphics
