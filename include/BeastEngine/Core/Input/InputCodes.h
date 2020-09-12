#pragma once
#include "BeastEngine/Core/Types.h"

namespace be
{
    enum class KeyCode : int16
    {
        INVALID = 0,
    };

    enum class MouseButtonCode : int16
    {
        INVALID = 0,
        BUTTON1 = 1,
        BUTTON2 = 2,
        BUTTON3 = 3,
        BUTTON4 = 4,
        BUTTON5 = 5,
        BUTTON6 = 6,

        BUTTON_LEFT = BUTTON1,
        BUTTON_MIDDLE = BUTTON3,
        BUTTON_RIGHT = BUTTON2
    };
} // namespace be
