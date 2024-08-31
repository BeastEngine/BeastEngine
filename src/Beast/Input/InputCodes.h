#pragma once
#include "Beast/Common/Types.h"

namespace be
{
    enum class KeyCode : uint8
    {
        INVALID = 0,
        Space,
        Quote,
        Comma,
        Minus,
        Period,
        Slash,
        Semicolon,
        EqualAdd,

        Alpha0,
        Alpha1,
        Alpha2,
        Alpha3,
        Alpha4,
        Alpha5,
        Alpha6,
        Alpha7,
        Alpha8,
        Alpha9,

        A,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K,
        L,
        M,
        N,
        O,
        P,
        Q,
        R,
        S,
        T,
        U,
        V,
        W,
        X,
        Y,
        Z,

        LeftBracket, /* [ */
        Backslash, /* \ */
        RightBracket, /* ] */
        Tilde, /* `~ */

        /* Additional keys */
        VolumeMute,
        VolumeDown,
        VolumeUp,
        MediaNextTrack,
        MediaPreviousTrack,
        MediaStop,
        MediaPlayAndPause,

        /* Function keys */
        Escape,
        Enter,
        Tab,
        Backspace,
        Insert,
        Delete,
        Right,
        Left,
        Down,
        Up,
        PageUp,
        PageDown,
        Home,
        End,
        CapsLock,
        ScrollLock,
        NumLock,
        PrintScreen,
        Pause,
        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,

        /* Numpad */
        Num0,
        Num1,
        Num2,
        Num3,
        Num4,
        Num5,
        Num6,
        Num7,
        Num8,
        Num9,
        NumDecimal,
        NumDivide,
        NumMultiply,
        NumSubtract,
        NumAdd,
        NumEnter,
        NumEqual,

        LeftShift,
        LeftControl,
        LeftAlt,
        LeftSuper,
        RightShift,
        RightControl,
        RightAlt,
        RightSuper,
        Menu,
        OEMSpecific,

        SIZE,
    };

    enum class MouseButtonCode : uint8
    {
        INVALID,
        BUTTON1,
        BUTTON2,
        BUTTON3,
        BUTTON4,
        BUTTON5,

        SIZE,

        BUTTON_LEFT = BUTTON1,
        BUTTON_MIDDLE = BUTTON3,
        BUTTON_RIGHT = BUTTON2,
    };
} // namespace be
