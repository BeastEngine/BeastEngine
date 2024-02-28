#pragma once
#include "Beast/Common/Types.h"
#include "Beast/Common/Utils/Hasher.h"

#include <uuid.h>
#include <random>

namespace be
{
    /**
     * @brief Generates new universally unique identifier (UUIDs) in the form of an integer.
     * 
     * @return New, unique Id
     */
    [[nodiscard]] inline Id GenerateUUID4()
    {
        std::random_device randomDevice;
        auto seed = std::array<int, std::mt19937::state_size>{};

        std::generate(std::begin(seed), std::end(seed), std::ref(randomDevice));
        std::seed_seq seedSequence(std::begin(seed), std::end(seed));
        std::mt19937 generatorEngine(seedSequence);
        uuids::uuid_random_generator uuidGenerator{generatorEngine};

        return Id{Hash(uuidGenerator().as_bytes())};
    }
} // namespace be
