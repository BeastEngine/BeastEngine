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
        /**
         * Converts std::string into WinAPI's wide string.
         *  This function is used to convert strings ONLY for WinAPI purposes.
         * 
         * @param narrowTitle
         * @return 
         */
        std::wstring ConvertWindowTitle(const std::string& narrowTitle) const;

        /**
         * Returns WinAPI's window style based on given Engine's window style.
         * 
         * @param windowStyle
         * @return 
         */
        uint32 GetWindowStyle(WindowStyle windowStyle) const;

        /**
         * Returns proper window's width and height.
         *  If window was created with WindowStyle::WINDOW_FULLSCREEN style, this function will return
         *  screen dimensions retrieved from the WinAPI. Otherwise, dimensions defined via WindowDescriptor
         *  struct will be returned.
         *  This function is used because we need to know actual display dimensions in order to make the window
         *  fullscreen.
         * 
         * @return 
         */
        IntVec2 GetWindowDimensions() const;

        /**
         * Calls WinAPI's ShowWindow(...) functions with appropriate parameters based on the provided
         *  window styles.
         */
        void DisplayWindow() const;

        /**
         * Turns on the fullscreen mode.
         */
        void ToggleToFullscreen() const;
        
        /**
         * These window procedure functions are used to setup the actual window procedure.
         *  These additional steps are required if we want the window procedure to be a member function.
         *  This setup function saves the WindowProcThunk to be the window's procedure.
         * 
         * @see WindowProcThunk
         * 
         * @param hWnd
         * @param uMsg
         * @param wParam
         * @param lParam
         * @return 
         */
        static LRESULT CALLBACK WindowProcSetup(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

        /**
         * Gets the pointer to the actual window class and calls the HandleWindowMessages(...) functions
         *  which handles the window's messages.
         * 
         * @see HandleWindowMessages
         * 
         * @param hWnd
         * @param uMsg
         * @param wParam
         * @param lParam
         * @return 
         */
        static LRESULT CALLBACK WindowProcThunk(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

        /**
         * Handles all window's messages and events.
         *  Dispatches them via AWindow::DispatchEvent(...) set of methods.
         *  It gets called automatically by the OS.
         * 
         * @param hWnd
         * @param uMsg
         * @param wParam
         * @param lParam
         * @return 
         */
        LRESULT HandleWindowMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) const;

    private:
        static constexpr const wchar_t* WINDOW_CLASS_NAME = L"BeastEngineWindow";

        /******************************************************/
        /******************* WINDOW STYLES ********************/
        using WindowFlagsMap = ConstexprMap<WindowStyle, uint32, 3>;
        static constexpr WindowFlagsMap::StorageType m_stylesMappings{
            {
                {WindowStyle::WINDOW_DEFUALT, WS_OVERLAPPEDWINDOW},
                {WindowStyle::WINDOW_BORDERLESS, WS_OVERLAPPED},
                {WindowStyle::WINDOW_FULLSCREEN, WS_OVERLAPPEDWINDOW},
            }};
        static constexpr WindowFlagsMap m_windowStylesMap{m_stylesMappings};
        /******************************************************/
        /******************************************************/

        /******************************************************/
        /**************** WINDOW DISPLAY PARAMS ***************/
        using WindowDisplayParamsMap = ConstexprMap<WindowStyle, uint32, 3>;
        static constexpr WindowDisplayParamsMap::StorageType m_displayParamsMappings{
            {
                {WindowStyle::WINDOW_DEFUALT, SW_SHOW},
                {WindowStyle::WINDOW_FULLSCREEN, SW_SHOW},
                {WindowStyle::WINDOW_BORDERLESS, SW_MAXIMIZE},
            }};
        static constexpr WindowDisplayParamsMap m_displayParamsMap{m_displayParamsMappings};
        /******************************************************/
        /******************************************************/

        HWND m_hwnd;
        HINSTANCE m_hInstance;
        WindowDescriptor m_descriptor;
    };
} // namespace be::internals

#endif
