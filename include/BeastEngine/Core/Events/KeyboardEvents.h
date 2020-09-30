#pragma once
#include "BeastEngine/Core/Input/InputCodes.h"
#include "BeastEngine/Core/Helpers.h"

namespace be
{
    enum class KeyboardEventType
    {
        EVENT_KEY_PRESSED,
        EVENT_KEY_HELD_DOWN,
        EVENT_KEY_RELEASED,
    };

    class KeyboardEvent
    {
    public:
        BE_IMPLEMENT_CONSTRUCTORS_DEFAULT(KeyboardEvent)

        /**
         * Initializes KeyboardEvent with provided code for the key the event concerns.
         * 
         * @param keyCode
         */
        KeyboardEvent(KeyCode keyCode, KeyboardEventType eventType)
            : m_keyCode(keyCode), m_type(eventType)
        {
        }
        virtual ~KeyboardEvent() = default;

        /**
         * Returns code of the key this event concerns.
         * 
         * @return KeyCode
         */
        KeyCode GetKey() const noexcept
        {
            return m_keyCode;
        }

        /**
         * Returns type of the keyboard event.
         * @see enum class KeyboardEventType
         * 
         * @return KeyboardEventType
         */
        KeyboardEventType GetType() const noexcept
        {
            return m_type;
        }

    private:
        KeyCode m_keyCode;
        KeyboardEventType m_type;
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

    class KeyHeldDownEvent final : public KeyboardEvent
    {
    public:
        /**
         * Initializes KeyHeldDownEvent with provided key code.
         * Creates KeyboardEvent with the same key code and with KeyboardEventType::EVENT_KEY_PRESSED event type
         * 
         * @param keyCode
         */
        KeyHeldDownEvent(KeyCode keyCode)
            : KeyboardEvent(keyCode, KeyboardEventType::EVENT_KEY_HELD_DOWN)
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
