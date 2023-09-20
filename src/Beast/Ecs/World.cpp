#include <Beast/Ecs/World.h>

namespace be
{
    Entity World::CreateEntity()
    {
        return m_registry.create();
    }

    bool World::IsValid(Entity entity) const
    {
        return m_registry.valid(entity);
    }
} // namespace be
