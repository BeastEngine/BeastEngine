#pragma once
#include "Beast/Physics/Types.h"
#include "Beast/Math/Types.h"

namespace be::physics
{
    class World
    {
    public:
        static World Create() noexcept;

        RigidBody CreateBoxRigidBody(const be::Vec2& position, const be::Vec2& dimensions, RigidBodyType type = RigidBodyType::STATIC) const noexcept;

        void SetLinearVelocity(RigidBody body, const be::Vec2& velocity) const noexcept;
        be::Vec2 GetPosition(RigidBody body) const noexcept;

        void Step() const noexcept;

    private:
        // TODO: We need a destructor to clear this up.
        b2WorldId m_world;
    };
} // namespace be::physics
