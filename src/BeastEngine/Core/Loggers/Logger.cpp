#include "BeastEngine/Core/Loggers/Logger.h"

namespace be
{
    static const char* EXCEPTION_MESSAGE = "Logger implementation cannot be nullptr!";
    Logger::Logger(LoggerImpl logger)
    {
        if (logger == nullptr)
        {
            throw std::invalid_argument(EXCEPTION_MESSAGE);
        }

        m_logger = logger;
    }
} // namespace be
