#pragma once
#include <Beast/Ecs/Types.h>

namespace be
{
    struct AccessList
    {
        using Get = Components<>;
        using Update = Components<>;
        using Add = Components<>;
        using Remove = Components<>;
    };
} // namespace be