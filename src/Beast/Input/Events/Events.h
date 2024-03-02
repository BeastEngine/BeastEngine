#pragma once
#include "Beast/Input/Events/KeyboardEvents.h"
#include "Beast/Input/Events/MouseEvents.h"

#include <functional>

namespace be
{
    /********************EVENT HANDLERS DEFINITIONS*************************/
    // Keyboard
    using KeyboardEventHandler = std::function<void(const KeyboardEvent&)>;

    // Mouse
    using MouseEventHandler = std::function<void(const MouseEvent&)>;
    using MouseWheelScrolledListener = std::function<void(WheelScrollDirection)>;

    // Window
    using WindowClosedEventHandler = std::function<void()>;
    /***********************************************************************/
} // namespace be
