#pragma once
#ifdef BE_PLATFORM_WINDOWS
    #include "Beast/Core/Windows/AWindow.h"
    #include "Beast/Core/PlatformSetup.h"
    #include "Beast/Core/DataStructures.h"

    #include <Beast/Common/Helpers.h>
    #include <functional>
    #include <unordered_map>
    #include <string>

namespace be::internals
{
    using MessageHandler = std::function<LRESULT(UINT, WPARAM, LPARAM)>;

    /**
     * @brief Represents Win32 application window.
     */
    class Win32Window final : public AWindow
    {
    public:
        CT_IMPLEMENT_ADDITIONAL_CONSTRUCTORS_DELETED(Win32Window);

        /**
         * @brief Creates WinAPI's window using given descriptor.
         * Registers WinAPI's window class and initializes HWND.
         * NOTE: windowClassName must be unique for every existing windows.
         * If other existing window already uses gien name, exception is thrown.
         * 
         * @param windowDescriptor - Struct containing details about window
         * @param windowClassName - Unique name of the WinAPI class to register for this window
         */
        Win32Window(const WindowDescriptor& windowDescriptor, const std::wstring_view windowClassName);

        /**
         * @brief Destroys WinAPI's window and unregisters its class. Deletes HWND.
         * You can now create new window with the same windowClassName
         * 
         * @see Win32Window()
         */
        ~Win32Window();

        /**
         * @see IWindow::ProcessInput()
         */
        void ProcessInput() override;

        /**
         * @brief Returns WinAPI's window handle.
         * 
         * @return HWND of the window
         */
        HWND GetNativeHandle() const noexcept;

    private:
        /**
         * @brief Converts std::string into WinAPI's wide string.
         * This function is used to convert strings ONLY for WinAPI purposes.
         * 
         * @param narrowTitle - 'Narrow' UTF-8 string
         * @return std::wstring representation of the given std::string
         */
        std::wstring ConvertWindowTitle(const std::string& narrowTitle) const;

        /**
         * @brief Returns WinAPI's window style based on given Engine's window style.
         * 
         * @param windowStyle - BeastEngine's window style to convert
         * @return Appropriate WS_* flag(s)
         */
        uint32 GetWindowStyle(WindowStyle windowStyle) const;

        /**
         * @brief Returns proper window's width and height.
         * If window was created with WindowStyle::WINDOW_FULLSCREEN style, this function will return
         * screen dimensions retrieved from the WinAPI. Otherwise, dimensions defined via WindowDescriptor struct will be returned.
         * This function is used because we need to know the actual display dimensions in order to make the window fullscreen.
         * 
         * @return IntVec2{x=width, y=height}
         */
        IntVec2 GetWindowDimensions() const;

        /**
         * @brief Calls WinAPI's ShowWindow(...) functions with appropriate parameters based on the provided window styles.
         */
        void DisplayWindow() const;

        /**
         * @brief Turns on the fullscreen mode.
         */
        void ToggleToFullscreen() const;

        /**
         * @brief These window procedure functions are used to setup the actual window procedure.
         * These additional steps are required if we want the window procedure to be a member function.
         * This setup function saves the WindowProcThunk to be the window's procedure.
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
         * @brief Gets the pointer to the actual window class and calls the HandleWindowMessages(...) function which handles the window's messages.
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
         * @brief Creates mappings between WinAPI messages and methods responsible for handling them.
         * Those handlers are later used to handle actual messages posted by OS.
         */
        void SetUpMessageHandlers();

        /**
         * @brief Maps WM_*MOUSEBUTTON* messages to appropriate handlers.
         */
        void SetUpMouseMessagesHandlers();

        /**
         * @brief Maps WM_*KEY* messages to appropriate handlers.
         */
        void SetUpKeyboardMessagesHandlers();

        /**
         * @brief Handles all window's messages.
         * If there is a message handler defined for given message, event is processed by that handler.
         * Otherwise, it calls the default handler provided by the WinAPI
         * This method is called automatically by the OS.
         * 
         * @see WindowProcSetup()
         * @sse WindowProcThunk()
         * 
         * @param hWnd
         * @param uMsg
         * @param wParam
         * @param lParam
         * @return 
         */
        LRESULT HandleWindowMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) const;

        /**
         * @brief Handles WM_*MOUSEBUTTONDOWN WinAPI messages.
         * Captures mouse cursor.
         * Retrieves pressed button code and dispatches mouse button events.
         * 
         * @param uMsg - WinAPI mouse button code associated with this event for basic mouse buttons.
         * @param wParam - WinAPI mouse button code associated with this event for additional mouse buttons.
         * @param lParam - Cursor coordinates
         * @return Result of message handling
         */
        LRESULT HandleMouseButtonDownMessages(UINT uMsg, WPARAM wParam, LPARAM lParam) const;

        /**
         * @brief Handles WM_*MOUSEBUTTONUP WinAPI messages.
         * Captures mouse cursor.
         * Retrieves pressed button code and dispatches mouse button events.
         * 
         * @param uMsg - WinAPI mouse button code associated with this event for basic mouse buttons.
         * @param wParam - WinAPI mouse button code associated with this event for additional mouse buttons.
         * @param lParam - Cursor coordinates
         * @return Result of message handling
         */
        LRESULT HandleMouseButtonUpMessages(UINT uMsg, WPARAM wParam, LPARAM lParam) const;

        /**
         * @brief Handles WM_KEYDOWN WinAPI messages.
         * Retrieves pressed button code.
         * Dispatches KeyPressedEvent if given key was pressed in this frame, or KeyHeldDownEvent otherwise.
         * 
         * @param uMsg - unused.
         * @param wParam - WinAPI key code associated with this event.
         * @param lParam - Information about associated key state
         * @return Result of message handling
         */
        LRESULT HandleKeyDownMessages(UINT uMsg, WPARAM wParam, LPARAM lParam) const;

        /**
         * @brief Checks if mouse buttons are held down.
         * Dispatches MouseButtonHeldDownEvent with valid MouseButtonCode if specific button is held down.
         */
        void ProcessHeldDownMessages() const;

        /**
         * @brief Returns mouse coordinates as IntVec2 extracted from the LPARAM of the WindowProc message.
         * 
         * @param lParam - LPARAM used to extract mouse coordinates from
         * @return
         */
        IntVec2 GetMouseCoordinates(LPARAM lParam) const
        {
            const auto point = MAKEPOINTS(lParam);
            return {point.x, point.y};
        }

        /**
         * @brief Returns info about whether the button represented by the given keyCode is currently pressed.
         * 
         * @see https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getkeystate
         * 
         * @param keyCode - The key to examine
         * @return True if the given key state's bit 16 is set to 1 and false otherwise
         */
        bool IsKeyPressed(int keyCode) const
        {
            return GetKeyState(keyCode) & KEY_STATE_IS_PRESSED;
        }

        /**
         * @brief Returns info about whether the virtual key's previous state represtened by the given LPARAM is VK_DOWN.
         * Uses bit number 30 of the given param to determine the value.
         * 
         * @see https://docs.microsoft.com/en-us/windows/win32/learnwin32/keyboard-input
         * 
         * @param lParam - LPARAM containing information about current WM_KEYDOWN virtual key
         * @return True if the given lParam's bit 30 is set to 1 and false otherwise
         */
        bool IsKeyHeldDown(LPARAM lParam) const
        {
            return lParam & KEY_STATE_IS_PRESSED_AND_REPEATED;
        }

    private:
        const std::wstring WINDOW_CLASS_NAME = L"";
        static constexpr const uint16 KEY_STATE_IS_PRESSED = 0x8000;
        static constexpr const LPARAM KEY_STATE_IS_PRESSED_AND_REPEATED = 0x40000000;

        HWND m_hwnd;
        HINSTANCE m_hInstance;
        WindowDescriptor m_descriptor;
        std::unordered_map<UINT, MessageHandler> m_messageHandlers;
    };
} // namespace be::internals

#endif
