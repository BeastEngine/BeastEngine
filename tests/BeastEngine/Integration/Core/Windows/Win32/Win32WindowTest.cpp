#ifdef BE_ENABLE_WINDOWS_TESTS
    #include "Win32WindowTest.h"

    #include <BeastEngine/Core/Windows/Win32/Win32Window.h>

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
} // namespace be::tests::integration
#endif
