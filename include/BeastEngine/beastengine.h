#pragma once
#include <string>

namespace be
{
    struct EngineConfig
    {
    };

    class BeastEngine
    {
    public:
        BeastEngine(EngineConfig config){};
        ~BeastEngine() = default;

        BeastEngine(const BeastEngine&) = delete;
        BeastEngine(BeastEngine&&) = delete;
        const BeastEngine& operator=(const BeastEngine&) = delete;
        BeastEngine&& operator=(BeastEngine&&) = delete;

        std::string SayHello() const;
    };
} // namespace be
