#include "BeastEngineTest.h"

#include <BeastEngine/Core/BeastEngine.h>

namespace be::tests::integration
{
    TEST_F(BeastEngineTest, CreateMainWindowWillReturnValidWindowWhenNoWindowFactoryImplementationProvided)
    {
        WindowHandleInstanceType hwndInstance = nullptr;
        auto windowParams = WindowDescriptor(hwndInstance);
        windowParams.dimensions = {0, 0};

        const auto sut = BeastEngine(EngineConfig());
        const auto result = sut.CreateNewWindow(windowParams);
        ASSERT_NE(nullptr, result);
    }
} // namespace be::tests::integration
