#include <iostream>

#include <BeastEngine/BeastEngine.h>

int main()
{
    const auto engine = be::BeastEngine(be::EngineConfig());
    engine.PrintInfo();

    std::getchar();

    return 0;
}
