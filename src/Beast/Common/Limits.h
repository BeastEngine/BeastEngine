#pragma once
#include "Beast/Common/TypeTraits.h"

#include <limits>

namespace be
{
    namespace limits
    {
        template<typename EnumType>
        inline constexpr auto MaxValue()
        {
            return std::numeric_limits<UnderlyingType<EnumType>>::max();
        }
    } // namespace limits
} // namespace be