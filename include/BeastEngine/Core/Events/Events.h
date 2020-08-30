#pragma once
#include "KeyboardEvents.h"

namespace be
{
    /********************EVENT HANDLERS DEFINITIONS*************************/
    // Keyboard
    using KeyboardEventHandler = std::function<void(KeyboardEvent event)>;
    using KeyPressedEventHandler = std::function<void(KeyPressedEvent event)>;
    using KeyDownEventHandler = std::function<void(KeyDownEvent event)>;
    using KeyReleasedEventHandler = std::function<void(KeyReleasedEvent event)>;
    /***********************************************************************/
} // namespace be
