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

        template<typename AL>
        using All = decltype(be::JoinTypes(AL::Get(), AL::Update(), AL::Add(), AL::Remove()));
    };
} // namespace be