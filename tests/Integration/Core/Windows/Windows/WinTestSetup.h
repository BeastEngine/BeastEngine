#ifdef BE_ENABLE_WINDOWS_TESTS
    #pragma comment(lib, "Rpcrt4.lib")

    #include "TestSetup.h"
    #include <BeastEngine/Core/Windows/Windows/WindowsWindow.h>
    #include <rpc.h>

namespace be::tests::integration
{
    class WinTest : public ::testing::Test
    {
    protected:
        UniquePtr<internals::WindowsWindow> GetWindow()
        {
            UUID uniqeClassNameId;
            UuidCreate(&uniqeClassNameId);

            // Make window invisible to avoid accidental mouse movement event triggering
            WindowDescriptor windowDescriptor(GetModuleHandle(nullptr));
            windowDescriptor.dimensions = {0, 0};

            auto window = CreateUniquePtr<be::internals::WindowsWindow>(windowDescriptor, GetUUIDAsString(uniqeClassNameId).c_str());
            ShowWindow(window->GetNativeHandle(), SW_SHOWNOACTIVATE);

            return window;
        }

    private:
        std::wstring GetUUIDAsString(const UUID& uuid)
        {
            return
                std::to_wstring(uuid.Data1)
                + std::to_wstring(uuid.Data2)
                + std::to_wstring(uuid.Data3)
                + std::to_wstring(uuid.Data4[0])
                + std::to_wstring(uuid.Data4[1])
                + std::to_wstring(uuid.Data4[2])
                + std::to_wstring(uuid.Data4[3])
                + std::to_wstring(uuid.Data4[4])
                + std::to_wstring(uuid.Data4[5])
                + std::to_wstring(uuid.Data4[6])
                + std::to_wstring(uuid.Data4[7]);
        }
    };
} // namespace be::tests::integration
#endif
