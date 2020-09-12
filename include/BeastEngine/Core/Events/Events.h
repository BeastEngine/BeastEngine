#pragma once
#include "KeyboardEvents.h"
#include "MouseEvents.h"

namespace be
{
    /********************EVENT HANDLERS DEFINITIONS*************************/
    // Keyboard
    using KeyboardEventHandler = std::function<void(const KeyboardEvent& event)>;

    // Mouse
    using MouseEventHandler = std::function<void(const MouseEvent& event)>;
    /***********************************************************************/
} // namespace be
