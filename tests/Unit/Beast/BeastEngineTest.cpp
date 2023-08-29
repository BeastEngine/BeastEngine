#include <Unit/Beast/BeastEngineTest.h>

#include <Beast/BeastEngine.h>

namespace be::tests::unit
{
    TEST_F(BeastEngineTest, CreateMainWindowWillUseDefinedWindowFactoryImplementationToCreateWindow)
    {
        auto windowFactory = CreateWindowFactoryMock();
        EXPECT_CALL(*windowFactory, Create)
            .Times(::testing::AtLeast(1));

        EngineConfig engineConfig;
        engineConfig.windowFactory = std::move(windowFactory);

        const auto sut = BeastEngine(std::move(engineConfig));
        sut.CreateNewWindow({m_wHInstance});
    }

    TEST_F(BeastEngineTest, CreateMainWindowWillUseDefinedWindowFactoryImplementationToCreateWindowAndReturnItsResult)
    {
        auto windowToReturn = CreateWindowMock();
        IWindow* expectedWindow = windowToReturn.get();

        auto windowFactory = CreateWindowFactoryMock();
        EXPECT_CALL(*windowFactory, Create)
            .WillOnce(testing::Return(testing::ByMove(std::move(windowToReturn))));

        EngineConfig engineConfig;
        engineConfig.windowFactory = std::move(windowFactory);

        const auto sut = BeastEngine(std::move(engineConfig));
        const Unique<IWindow> actualWindow = sut.CreateNewWindow({m_wHInstance});

        ASSERT_EQ(expectedWindow, actualWindow.get());
    }
} // namespace be::tests::unit
