#pragma once
#include <entt/entity/fwd.hpp>

#include <type_traits>

namespace be
{
    using Entity = entt::entity;
    using Registry = entt::registry;

    template<typename... Args>
    struct TypeList
    {
    };

    template<typename... ComponentTypes>
    struct Components : public TypeList<ComponentTypes...>
    {
        template<typename Component>
        struct Contains
        {
            static constexpr auto value = (std::is_same_v<Component, ComponentTypes> || ...);
        };
    };

    template<typename... T>
    struct JoinComponents;

    template<typename... A, typename... B>
    struct JoinComponents<Components<A...>, Components<B...>>
    {
        using type = Components<A..., B...>;
    };

    template<typename... A, typename... B, typename... C>
    struct JoinComponents<Components<A...>, Components<B...>, C...>
    {
        using type =
            typename JoinComponents<typename JoinComponents<Components<A...>, Components<B...>>::type, C...>::type;
    };

    template<typename... T>
    using JoinComponentsT = typename JoinComponents<T...>::type;
} // namespace be