#ifdef BE_ENABLE_WINDOWS_TESTS
    #include <TestSetup.hpp>

    #include <Beast/Windows/Win32/Win32Window.hpp>
    #include <Beast/Common/UUIDGenerator.hpp>

namespace be::tests::integration
{
    class Win32Test : public ::testing::Test
    {
    protected:
        auto GetWindowDescriptor()
        {
            // Make window invisible to avoid accidental mouse movement event triggering
            WindowDescriptor windowDescriptor(GetModuleHandle(nullptr));
            windowDescriptor.dimensions = {0, 0};

            return windowDescriptor;
        }

        auto GetWindow()
        {
            Id uniqeClassNameId = GenerateUUID4();

            auto window = MakeUnique<be::internals::Win32Window>(GetWindowDescriptor(), GetIdAsWString(uniqeClassNameId).c_str());
            ShowWindow(window->GetHandle(), SW_SHOWNOACTIVATE);

            return window;
        }

    private:
        std::wstring GetIdAsWString(Id id)
        {
            return std::to_wstring(id.Raw());
        }
    };
} // namespace be::tests::integration
#endif
