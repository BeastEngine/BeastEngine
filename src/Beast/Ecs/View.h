#pragma once
#include "Beast/Ecs/Types.h"
#include "Beast/Common/Helpers.h"

#include <entt/entity/registry.hpp>

#include <iostream>
#include <typeinfo>

namespace be
{
    template<typename BaseAccessList>
    class View final
    {
        friend class World;

    private:
        using AL = BaseAccessList;

        using Get = AL::Get;
        using Update = AL::Update;
        using Add = AL::Add;
        using Remove = AL::Remove;

        using Included = JoinComponentsT<Get, Update, Remove>;
        using Excluded = JoinComponentsT<Add>;

    public:
        constexpr auto begin() const
        {
            return m_view.begin();
        }

        constexpr auto end() const
        {
            return m_view.end();
        }

        template<typename Component>
        constexpr const Component& GetComponent(be::Entity entity) const
        {
            static_assert(ComponentsHave<BaseAccessList::Get, Component> || ComponentsHave<BaseAccessList::Update, Component>);
            return m_view.get<const Component>(entity);
        }

        template<typename Component>
        constexpr Component& UpdateComponent(be::Entity entity) const
        {
            static_assert(ComponentsHave<BaseAccessList::Update, Component>);
            return m_view.get<Component>(entity);
        }

        template<typename Component>
        constexpr void AddComponent(be::Entity entity, Component&& component) const
        {
            static_assert(ComponentsHave<BaseAccessList::Add, Component>);
            m_registry.emplace<Component>(entity, std::move(component));
        }

        template<typename Component>
        constexpr void RemoveComponent(be::Entity entity) const
        {
            static_assert(ComponentsHave<BaseAccessList::Remove, Component>);
            m_registry.remove<Component>(entity);
        }

        template<typename Component>
        constexpr bool HasComponent(be::Entity entity) const
        {
            return m_registry.try_get<Component>(entity) != nullptr;
        }

        constexpr std::size_t Size() const
        {
            return m_view.size();
        }

    private:
        template<typename... IncludedComponents, typename... ExcludedComponents>
        constexpr static auto Init(Components<IncludedComponents...>, Components<ExcludedComponents...>, Registry& reg)
        {
            if constexpr (sizeof...(IncludedComponents) == 0)
            {
                return reg.view<entt::entity>(entt::exclude<ExcludedComponents...>);
            }
            else
            {
                return reg.view<IncludedComponents...>(entt::exclude<ExcludedComponents...>);
            }
        }

        using ViewType =
            decltype(Init(
                Included(),
                Excluded(),
                std::declval<Registry&>()
            ));

        constexpr explicit View(Registry& registry)
            : m_registry(registry), m_view(Init(Included(), Excluded(), m_registry))
        {
        }

        BE_IMPLEMENT_ADDITIONAL_CONSTRUCTORS_DELETED(View<BaseAccessList>);

    private:
        Registry& m_registry;
        ViewType m_view;
    };
} // namespace be
