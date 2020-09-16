#pragma once
#ifdef BE_PLATFORM_WINDOWS
    #include "BeastEngine/Core/Windows/AWindow.h"
    #include "BeastEngine/Core/PlatformSetup.h"
    #include "BeastEngine/Core/DataStructures.h"

namespace be::internals
{
    class WindowsWindow final : public AWindow
    {
    public:
        WindowsWindow(const WindowDescriptor& windowDescriptor);
        ~WindowsWindow();
        virtual void ProcessInput() override;

    private:
        std::wstring ConvertWindowTitle(const std::string& narrowTitle) const;
        uint32 GetWindowFlags(const std::vector<WindowFlags>& userDefinedFlags) const;
        
        static LRESULT CALLBACK WindowProcSetup(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        static LRESULT CALLBACK WindowProcThunk(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        LRESULT HandleWindowMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) const;

    private:
        using WindowFlagsMap = ConstexprMap<WindowFlags, uint32, 1>;

        HWND m_hwnd;
        HINSTANCE m_hInstance;

        static constexpr const wchar_t* WINDOW_CLASS_NAME = L"BeastEngineWindow";
        static constexpr WindowFlagsMap::StorageType m_mapData{{{WindowFlags::WINDOW_RESIZABLE, WS_OVERLAPPEDWINDOW}}};
        static constexpr WindowFlagsMap m_windowFlagsMap{m_mapData};
    };
} // namespace be::internals

#endif
