#include <iostream>

#include <BeastEngine/BeastEngine.h>
#include <BeastEngine/Loggers/StaticLogger.h>

int main()
{
    const auto engine = be::BeastEngine(be::EngineConfig());
    engine.PrintInfo();

    std::getchar();

    return 0;
}
