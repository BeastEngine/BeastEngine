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

    template<typename... Args1, typename... Args2, typename... Args3, typename... Args4>
    constexpr static auto JoinTypes(Components<Args1...>, Components<Args2...>, Components<Args3...>, Components<Args4...>)
    {
        return Components<Args1..., Args2..., Args3..., Args4...>{};
    }
}