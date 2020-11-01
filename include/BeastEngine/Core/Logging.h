#pragma once
#include "BeastEngine/Core/Loggers/LoggersFactories.h"
#include "BeastEngine/Core/Loggers/Logger.h"

#ifndef BE_DEBUG_MESSAGE
    #define BE_DEBUG_MESSAGE(message) "[{}:{}] *** " message " ***", __FILE__, __LINE__
#endif
