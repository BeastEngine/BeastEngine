#include "BeastEngine/beastengine.h"
#include "spdlog/spdlog.h"

const char* Greet()
{
	spdlog::info("Welcome to spdlog version {}.{}.{}  !", SPDLOG_VER_MAJOR, SPDLOG_VER_MINOR, SPDLOG_VER_PATCH);
	
	return "Hello World!";
}
