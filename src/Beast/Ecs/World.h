#include <Beast/Ecs/Types.h>
#include <Beast/Ecs/View.h>
#include <Beast/Ecs/AccessList.h>

#include <concepts>

namespace be
{
    class World final
    {
    public:
        [[nodiscard]] Entity CreateEntity();
        [[nodiscard]] bool IsValid(Entity entity) const;

        template<typename Component>
        constexpr void AddComponent(Entity entity, Component&& component)
        {
            m_registry.emplace<Component>(entity, std::move(component));
        }

        template<typename AL> requires std::derived_from<AL, be::AccessList>
        constexpr auto CreateView()
        {
            using VType = View<AL>;
            return VType(VType::Init(VType::All(), m_registry), m_registry);
        }

    private:
        Registry m_registry;
    };
} // namespace be