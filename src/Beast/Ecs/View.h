#pragma once
#include "Beast/Ecs/Types.h"
#include "Beast/Ecs/Components/Core.h"

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

    public:
        using AL = BaseAccessList;

    private:
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
            static_assert(ComponentsHave<Get, std::decay_t<Component>> || ComponentsHave<Update, std::decay_t<Component>>, "Component must be in either the AL::Get or AL::Update list!");
            return m_view.get<const Component>(entity);
        }

        template<typename Component>
        constexpr Component& UpdateComponent(be::Entity entity) const
        {
            static_assert(ComponentsHave<Update, std::decay_t<Component>>, "Component must be in the AL::Update list!");
            return m_view.get<Component>(entity);
        }

        template<typename Component>
        constexpr void AddComponent(be::Entity entity, Component&& component) const
        {
            static_assert(ComponentsHave<Add, std::decay_t<Component>>, "Component must be in the AL::Add list!");
            m_registry.emplace<std::decay_t<Component>>(entity, std::forward<Component>(component));
        }

        template<typename Component>
        constexpr void RemoveComponent(be::Entity entity) const
        {
            static_assert(ComponentsHave<Remove, std::decay_t<Component>>, "Component must be in the AL::Remove list!");
            m_registry.remove<Component>(entity);
        }

        template<typename Component>
        constexpr bool HasComponent(be::Entity entity) const
        {
            return m_registry.try_get<Component>(entity) != nullptr;
        }

        template<typename Tag>
        constexpr void AddTag(be::Entity entity) const
        {
            static_assert(ComponentsHave<Add, std::decay_t<Tag>>, "Tag must be in the AL::Add list!");
            m_registry.emplace<std::decay_t<Tag>>(entity);
        }

        template<typename Tag>
        constexpr void RemoveTag(be::Entity entity) const
        {
            static_assert(ComponentsHave<Remove, std::decay_t<Tag>>, "Tag must be in the AL::Remove list!");
            m_registry.remove<Tag>(entity);
        }

        template<typename Tag>
        constexpr bool HasTag(be::Entity entity) const
        {
            return m_registry.all_of<Tag>(entity);
        }

        constexpr be::Entity CreateEntity(be::Transform transform = {}) const
        {
            auto entity = m_registry.create();
            AddComponent<be::Transform>(entity, std::move(transform));

            return entity;
        }

        constexpr std::size_t EntitiesCount() const
        {
            return std::distance(m_view.begin(), m_view.end());
        }

        template<typename Component>
        constexpr std::size_t EntitiesCount() const
        {
            const auto view = m_registry.view<Component>();
            return std::distance(view.begin(), view.end());
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
