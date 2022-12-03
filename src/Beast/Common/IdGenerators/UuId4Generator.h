#pragma once
#include <Beast/Common/IdGenerators/IUuIdGenerator.h>

namespace be
{
    /**
     * @brief Implementation of the IuuIdGenerator interface responsible for creating UUID version 4 as described here: https://en.wikipedia.org/wiki/Universally_unique_identifier#Versions.
     */
    class UuId4Generator final : public IUuIdGenerator
    {
    public:
        [[nodiscard]] Id Generate() const override;
    };
} // namespace be
