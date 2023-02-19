#ifdef BE_ENABLE_WINDOWS_TESTS
    #include <TestSetup.h>

    #include <Beast/Windows/Win32/Win32Window.h>
    #include <Beast/Common/IdGenerators/UuId4Generator.h>

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
            UuId4Generator idGenerator;
            Id uniqeClassNameId = idGenerator.Generate();

            auto window = CreateUnique<be::internals::Win32Window>(GetWindowDescriptor(), GetIdAsWString(uniqeClassNameId).c_str());
            ShowWindow(window->GetNativeHandle(), SW_SHOWNOACTIVATE);

            return window;
        }

    private:
        std::wstring GetIdAsWString(Id id)
        {
            return std::to_wstring(id);
        }
    };
} // namespace be::tests::integration
#endif
