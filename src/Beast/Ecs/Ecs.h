#pragma once
#include <Beast/Ecs/World.h>
#include <Beast/Ecs/SystemsScheduler.h>

namespace be
{
    struct Ecs
    {
        World world;
        SystemsScheduler scheduler{world};
    };
} // namespace be