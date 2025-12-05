#include "Integration/Beast/Config.h"

#include <gtest/gtest.h>

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);

    const be::fs::Path exePath{argv[0]};
    be::tests::integration::InitConfig(exePath.parent_path() / "data");

    return RUN_ALL_TESTS();
}
