#pragma once
#include <entt/entt.hpp>

#include <tuple>
#include <type_traits>

namespace be
{
    using Entity = entt::entity;
    using Registry = entt::registry;

    inline constexpr Entity NULL_ENTITY = entt::null;

    constexpr auto ToIntegral(Entity entity)
    {
        return entt::to_integral(entity);
    }

    template<typename... ComponentTypes>
    using Components = entt::type_list<ComponentTypes...>;

    template<typename Components, typename Component>
    inline constexpr bool ComponentsHave = entt::type_list_contains_v<Components, Component>;

    template<typename... T>
    using JoinComponentsT = typename entt::type_list_unique_t<typename entt::type_list_cat_t<T...>>;
} // namespace be