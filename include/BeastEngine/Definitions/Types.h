#pragma once
#include <memory>
#include <spdlog/logger.h>

namespace be
{
    using ILogger = std::shared_ptr<spdlog::logger>;
}
