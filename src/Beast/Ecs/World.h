#pragma once
#include "Beast/Ecs/Types.h"
#include "Beast/Ecs/View.h"
#include "Beast/Ecs/AccessList.h"

#include "Beast/Common/Types.h"

#include <concepts>

namespace be
{
    class World final
    {
        struct CreateEntityAccessList final : BaseAccessList
        {
            using Add = Components<Transform>;
        };

    public:
        template<typename AL = World::CreateEntityAccessList>
        [[nodiscard]] constexpr auto CreateView() requires std::derived_from<AL, be::BaseAccessList>
        {
            return View<AL>(m_registry);
        }

    private:
        Registry m_registry;
    };
} // namespace be
