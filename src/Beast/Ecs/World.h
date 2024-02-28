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
        struct CreateEntityAL final : BaseAccessList
        {
            using Add = Components<Transform>;
        };

    public:
        /**
         * @brief Creates and returns a view containing entities with/without the components defined in the given \c AL template argument.
         * If not argument is provided, returns a view capable of creating entities.
         * 
         * @details There are some rules on how the entities are filtered/included based on the given access list.
         *  - The view contains ALL entities that have ALL of the components from Get, Update and Remove list.
         *      Leaving any of those empty will result in not taking that components into
         *      consideration, meaning that an access list with only `Get` will work as expected - will return all entities with all of the components from the `Get` lists.
         *  - The view contains ALL entities that don't have the ANY of the components from the Add list.
         */
        template<typename AL = World::CreateEntityAL>
        [[nodiscard]] constexpr auto CreateView() requires std::derived_from<AL, be::BaseAccessList>
        {
            return View<AL>(m_registry);
        }

    private:
        Registry m_registry;
    };
} // namespace be
