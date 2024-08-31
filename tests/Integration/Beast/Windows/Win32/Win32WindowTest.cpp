#ifdef BE_ENABLE_WINDOWS_TESTS
    #include <Integration/Beast/Windows/Win32/Win32WindowTest.hpp>

    #include <Beast/Windows/Win32/Win32Window.hpp>

namespace be::tests::integration
{
    TEST_F(Win32WindowTest, ConstructorWillThrowExceptionIfSecondWindowWithTheSameNameCreatedWhileFirstOneStillExists)
    {
        const wchar_t* windowClassName = L"WindowClass";

        const auto firstWindow = internals::Win32Window(GetWindowDescriptor(), windowClassName);
        ASSERT_THROW(internals::Win32Window(GetWindowDescriptor(), windowClassName), std::runtime_error);
    }

    TEST_F(Win32WindowTest, ConstructorWillNotThrowExceptionIfSecondWindowWithTheSameNameCreatedAfterFirstOneWasDestroyed)
    {
        const wchar_t* windowClassName = L"WindowClass";

        ASSERT_NO_THROW(internals::Win32Window(GetWindowDescriptor(), windowClassName)); // Window created and destroyed
        ASSERT_NO_THROW(internals::Win32Window(GetWindowDescriptor(), windowClassName));
    }

    TEST_F(Win32WindowTest, ProcessInputWillProperlyHandleCloseWindowEvent)
    {
        auto sut = GetWindow();
        ASSERT_FALSE(sut->ShouldClose());

        PostMessage(sut->GetHandle(), WM_CLOSE, NULL, NULL);
        sut->ProcessInput();

        ASSERT_TRUE(sut->ShouldClose());
    }
} // namespace be::tests::integration
#endif
