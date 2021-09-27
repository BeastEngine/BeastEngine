#include "BeastEngineTest.h"

#include <BeastEngine/Core/BeastEngine.h>

namespace be::tests::unit
{
    TEST_F(BeastEngineTest, CreateMainWindowWillUseDefinedWindowFactoryImplementationToCreateWindow)
    {
        UniquePtr<IWindowFactory> windowFactory = CreateUniquePtr<WindowFactoryMock>();
        WindowFactoryMock& windowFactoryMock = *(static_cast<WindowFactoryMock*>(windowFactory.get()));
        EXPECT_CALL(windowFactoryMock, CreateProxy)
            .Times(::testing::AtLeast(1));

        EngineConfig engineConfig;
        engineConfig.windowFactory = std::move(windowFactory);

        const auto sut = BeastEngine(std::move(engineConfig));
        sut.CreateMainWindow({m_wHInstance});
    }

    TEST_F(BeastEngineTest, CreateMainWindowWillReturnValidWindowWhenNoWindowFactoryImplementationProvided)
    {
        auto windowParams = WindowDescriptor(m_wHInstance);
        windowParams.dimensions = {0, 0};

        const auto sut = BeastEngine(EngineConfig());
        const auto result = sut.CreateMainWindow(windowParams);
        ASSERT_NE(nullptr, result);
    }

    TEST_F(BeastEngineTest, CreateMainWindowWillUseDefinedWindowFactoryImplementationToCreateWindowAndReturnItsResult)
    {
        // Note that this pointer will be passed to the UniquePtr
        // when the WindowFactoryMock::Create method will be called.
        // Thus, the memory will get released once this UniquePtr goes out of scope.
        // @see BeastEngineTest.h::WindowFactoryMock::Create
        IWindow* expectedWindow = new WindowMock();

        UniquePtr<IWindowFactory> windowFactory = CreateUniquePtr<WindowFactoryMock>();
        WindowFactoryMock& windowFactoryMock = *(static_cast<WindowFactoryMock*>(windowFactory.get()));
        EXPECT_CALL(windowFactoryMock, CreateProxy)
            .WillOnce(testing::Return(expectedWindow));

        EngineConfig engineConfig;
        engineConfig.windowFactory = std::move(windowFactory);

        const auto sut = BeastEngine(std::move(engineConfig));
        const UniquePtr<IWindow> actualWindow = sut.CreateMainWindow({m_wHInstance});

        ASSERT_EQ(expectedWindow, actualWindow.get());
    }
} // namespace be::tests::unit
