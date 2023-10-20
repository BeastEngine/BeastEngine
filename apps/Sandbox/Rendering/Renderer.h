#pragma once
#include <Beast/Graphics/Components.h>
#include <Beast/Graphics/IContext.h>

#include <Beast/Ecs/AccessList.h>
#include <Beast/Common/Types.h>

namespace be
{
    template<typename>
    class View;
}

class Renderer final
{
public:
    struct AccessList : be::BaseAccessList
    {
        using Get = be::Components<be::Sprite>;
    };

    explicit Renderer(be::Shared<be::graphics::IContext> context);
    void Run(const be::View<AccessList>& view);

private:
    struct RenderingResource
    {
        be::Texture texture;
        be::Material material;
    };

    std::vector<RenderingResource> m_resources;
    be::Shared<be::graphics::IContext> m_context;
};
