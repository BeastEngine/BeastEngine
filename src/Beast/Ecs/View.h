#pragma once
#include <Beast/Ecs/Types.h>

#include <entt/entity/registry.hpp>
#include <iostream>
#include <typeinfo>

namespace be
{
    template<typename AccessList>
    class View
    {
    private:
        using AL = AccessList;
        using Reg = Registry&;

        template<typename... ViewComponents>
        constexpr static auto init(Components<ViewComponents...>, Reg reg)
        {
            return reg.view<ViewComponents...>();
        }

        using ViewType =
            decltype(init(
                AL::template All<AL>(),
                std::declval<Reg>()
            ));

    public:
        /*constexpr View(ViewType view)
            : m_view(view)
        {
        }*/

        constexpr View(Reg reg)
            : m_view(init(
                  AL::template All<AL>(),
                  reg
              ))
        {
        }

        constexpr auto begin() const
        {
            return m_view.begin();
        }

        constexpr auto end() const
        {
            return m_view.end();
        }

        template<typename Component>
        constexpr const Component& Get(be::Entity ent) const
        {
            static_assert(AccessList::Get::Contains<Component>::value);
            return m_view.get<const Component>(ent);
        }

        template<typename Component>
        constexpr Component& Update(be::Entity ent) const
        {
            static_assert(AccessList::Update::Contains<Component>::value);
            return m_view.get<Component>(ent);
        }

    private:
        ViewType m_view;
    };
} // namespace be