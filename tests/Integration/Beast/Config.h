#pragma once
#include <Beast/Common/Filesystem/Types.h>

namespace be::tests::integration
{
    struct Config
    {
        fs::Path dataBasePath = "NOT_SETUP";
    };

    void InitConfig(fs::Path dataBasePath);
    const Config& GetConfig();
} // namespace be::tests::integration