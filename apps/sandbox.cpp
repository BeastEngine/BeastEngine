#include <iostream>
#include <BeastEngine/BeastEngine.h>
#include <BeastEngine/Loggers/StaticLogger.h>

int main()
{
    const auto engine = be::BeastEngine(be::EngineConfig());

    std::getchar();

    return 0;
}
