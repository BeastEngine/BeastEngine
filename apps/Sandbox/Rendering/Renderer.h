#pragma once
#include <Beast/Ecs/Components/Core.h>
#include <Beast/Ecs/Components/Graphics.h>

#include <Beast/Graphics/Graphics.h>

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
        using Get = be::Components<be::Sprite, be::Transform>;
    };

    explicit Renderer(be::Shared<be::graphics::Graphics> graphics);
    void Run(const be::View<AccessList>& view);

private:
    struct RenderingResource
    {
        be::Texture texture;
        be::Material material;
    };

    std::vector<RenderingResource> m_resources;
    be::Shared<be::graphics::Graphics> m_graphics;
};
