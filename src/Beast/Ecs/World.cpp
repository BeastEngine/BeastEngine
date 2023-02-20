#include <Beast/Ecs/World.h>

namespace be
{
    Entity World::CreateEntity() const
    {
        return NULL_ENTITY;
    }

    bool World::IsValid(Entity) const
    {
        return false;
    }
}