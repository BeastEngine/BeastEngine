#include "Beast/Physics/World.h"
#include "Beast/Debug.h"

#include <box2d/box2d.h>

namespace be::physics
{
    static constexpr b2BodyType Convert(RigidBodyType type) noexcept
    {
        switch (type)
        {
        case be::physics::RigidBodyType::STATIC:
            return b2_staticBody;
        case be::physics::RigidBodyType::KINEMATIC:
            return b2_kinematicBody;
        case be::physics::RigidBodyType::DYNAMIC:
            return b2_dynamicBody;
        default:
            BE_ASSERT_ALWAYS(false);
            return {};
        }
    }

    static constexpr b2Vec2 Convert(const be::Vec2& vec) noexcept
    {
        return {vec.x, vec.y};
    }

    static constexpr be::Vec2 Convert(const b2Vec2& vec) noexcept
    {
        return {vec.x, vec.y};
    }

    World World::Create() noexcept
    {
        b2WorldDef worldDef = b2DefaultWorldDef();
        worldDef.gravity.x = 0.0f;
        worldDef.gravity.y = 0.0f;

        World world{};
        world.m_world = b2CreateWorld(&worldDef);

        return world;
    }

    RigidBody World::CreateBoxRigidBody(const be::Vec2& position, const be::Vec2& dimensions, RigidBodyType type) const noexcept
    {
        b2BodyDef bodyDef = b2DefaultBodyDef();
        bodyDef.type = Convert(type);
        bodyDef.position = Convert(position);

        b2BodyId rigidBody = b2CreateBody(m_world, &bodyDef);
        b2Polygon groundBox = b2MakeBox(dimensions.x, dimensions.y);

        b2ShapeDef groundShapeDef = b2DefaultShapeDef();
        b2CreatePolygonShape(rigidBody, &groundShapeDef, &groundBox);

        return rigidBody;
    }

    void World::SetLinearVelocity(RigidBody body, const be::Vec2& velocity) const noexcept
    {
        b2Body_SetLinearVelocity(body, Convert(velocity));
    }

    be::Vec2 World::GetPosition(RigidBody body) const noexcept
    {
        return Convert(b2Body_GetPosition(body));
    }

    void World::Step() const noexcept
    {
        static constexpr float timeStep = 1.0f / 60.0f;
        static constexpr int subStepCount = 4;

        b2World_Step(m_world, timeStep, subStepCount);
    }
} // namespace be::physics
