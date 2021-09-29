#include "BeastEngine/Core/Loggers/Logger.h"

#include <Common/Exceptions.h>

namespace be
{
    constexpr static const char* EXCEPTION_MESSAGE = "Logger implementation cannot be nullptr!";
    Logger::Logger(LoggerImpl logger)
    {
        if (logger == nullptr)
        {
            throw std::invalid_argument(CT_EXCEPTION_MESSAGE(EXCEPTION_MESSAGE));
        }

        m_logger = logger;
    }
} // namespace be
