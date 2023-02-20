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

        using Get = AL::Get;
        using Update = AL::Update;
        using Add = AL::Add;
        using Remove = AL::Remove;

        using All = JoinComponentsT<Get, Update, Add, Remove>;

        template<typename... ViewComponents>
        constexpr static auto init(Components<ViewComponents...>, Reg reg)
        {
            return reg.view<ViewComponents...>();
        }

        using ViewType =
            decltype(init(
                All(),
                std::declval<Reg>()
            ));

    public:
        /*constexpr View(ViewType view)
            : m_view(view)
        {
        }*/

        constexpr View(Reg reg)
            : m_view(init(
                  All(),
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
        constexpr const Component& GetComponent(be::Entity ent) const
        {
            static_assert(AccessList::Get::Contains<Component>::value);
            return m_view.get<const Component>(ent);
        }

        template<typename Component>
        constexpr Component& UpdateComponent(be::Entity ent) const
        {
            static_assert(AccessList::Update::Contains<Component>::value);
            return m_view.get<Component>(ent);
        }

    private:
        ViewType m_view;
    };
} // namespace be