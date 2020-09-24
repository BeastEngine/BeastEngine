#pragma once
#include "Unit/TestSetup.h"

#include <spdlog/logger.h>
#include <BeastEngine/Core/Logging.h>

namespace be::tests::unit
{
    class LoggersFactoryTest : public ::testing::TestWithParam<LoggerType>
    {
    protected:
    };
} // namespace be::tests::unit
