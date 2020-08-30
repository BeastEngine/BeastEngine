#pragma once
#include "BeastEngine/Core/Input/InputCodes.h"
#include "BeastEngine/Core/Helpers.h"

namespace be
{
    enum class KeyboardEventType
    {
        EVENT_KEY_PRESSED,
        EVENT_KEY_DOWN,
        EVENT_KEY_RELEASED,
    };

    class KeyboardEvent
    {
    public:
        IMPLEMENT_CONSTRUCTORS_DEFAULT(KeyboardEvent)

        /**
         * Initializes KeyboardEvent with provided code for the key the event concerns.
         * 
         * @param keyCode
         */
        KeyboardEvent(KeyCode keyCode, KeyboardEventType eventType)
            : m_keyCode(keyCode), m_eventType(eventType)
        {
        }
        virtual ~KeyboardEvent() = default;

        /**
         * Returns code of the key this event concerns.
         * 
         * @return KeyCode
         */
        KeyCode GetKeyCode() const noexcept
        {
            return m_keyCode;
        }

        /**
         * Returns type of the keyboard event.
         * @see enum class KeyboardEventType
         * 
         * @return KeyboardEventType
         */
        KeyboardEventType GetEventType() const noexcept
        {
            return m_eventType;
        }

    private:
        KeyCode m_keyCode;
        KeyboardEventType m_eventType;
    };

    class KeyPressedEvent final : public KeyboardEvent
    {
    public:
        /**
         * Initializes KeyPressedEvent with provided key code.
         * Creates KeyboardEvent with the same key code and with KeyboardEventType::EVENT_KEY_PRESSED event type
         * 
         * @param keyCode
         */
        KeyPressedEvent(KeyCode keyCode)
            : KeyboardEvent(keyCode, KeyboardEventType::EVENT_KEY_PRESSED)
        {
        }
    };

    class KeyDownEvent final : public KeyboardEvent
    {
    public:
        /**
         * Initializes KeyDownEvent with provided key code.
         * Creates KeyboardEvent with the same key code and with KeyboardEventType::EVENT_KEY_PRESSED event type
         * 
         * @param keyCode
         */
        KeyDownEvent(KeyCode keyCode)
            : KeyboardEvent(keyCode, KeyboardEventType::EVENT_KEY_DOWN)
        {
        }
    };

    class KeyReleasedEvent final : public KeyboardEvent
    {
    public:
        /**
         * Initializes KeyReleasedEvent with provided key code.
         * Creates KeyboardEvent with the same key code and with KeyboardEventType::EVENT_KEY_PRESSED event type
         * 
         * @param keyCode
         */
        KeyReleasedEvent(KeyCode keyCode)
            : KeyboardEvent(keyCode, KeyboardEventType::EVENT_KEY_RELEASED)
        {
        }
    };
} // namespace be
