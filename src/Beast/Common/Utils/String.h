#pragma once
#include <Beast/Common/Types.h>

#include <string>

namespace be
{
    inline std::string ToString(Id id)
    {
        return std::to_string(ToUnderlying(id));
    }
}
