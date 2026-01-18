#include "Config.h"

namespace be::tests::integration
{
    static Config g_config;

    void InitConfig(fs::Path dataBasePath)
    {
        g_config.dataBasePath = std::move(dataBasePath);
    }

    const Config& GetConfig()
    {
        return g_config;
    }
} // namespace be::tests::integration