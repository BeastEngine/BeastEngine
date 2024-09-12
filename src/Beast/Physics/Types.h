#pragma once
#include <box2d/id.h>

namespace be::physics
{
    using RigidBody = b2BodyId;

    enum class RigidBodyType
    {
        STATIC,
        KINEMATIC,
        DYNAMIC
    };
} // namespace be::physics
