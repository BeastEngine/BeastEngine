#pragma once
#include "Beast/Input/InputCodes.hpp"

namespace be
{
    enum class KeyboardEventType
    {
        EVENT_KEY_PRESSED, // Pressed once, in the current frame
        EVENT_KEY_HELD_DOWN, // Was pressed and is now held down
        EVENT_KEY_RELEASED, // Released in the current frame
    };

    /**
     * @brief Stores single keybord event data.
     */
    struct KeyboardEvent final
    {
    public:
        /**
         * @brief Initializes KeyboardEvent with its key code and type.
         * 
         * @param keyCode - Code of the key that triggered this event
         * @param eventType - Type of triggered event
         */
        constexpr KeyboardEvent(KeyCode keyCode, KeyboardEventType eventType) noexcept
            : key(keyCode), type(eventType)
        {
        }

        /**
         * @brief Creates key pressed event with code of the key that triggered the event.
         * 
         * @param code - Code of the triggering key
         * @return 
         */
        static constexpr auto KeyPressed(KeyCode code) noexcept
        {
            return KeyboardEvent(code, KeyboardEventType::EVENT_KEY_PRESSED);
        }

        /**
         * @brief Creates key held down event with code of the key that triggered the event.
         * 
         * @param code - Code of the triggering key
         * @return 
         */
        static constexpr auto KeyHeldDown(KeyCode code) noexcept
        {
            return KeyboardEvent(code, KeyboardEventType::EVENT_KEY_HELD_DOWN);
        }

        /**
         * @brief Creates key released event with code of the key that triggered the event.
         * 
         * @param code - Code of the triggering key
         * @return 
         */
        static constexpr auto KeyReleased(KeyCode code) noexcept
        {
            return KeyboardEvent(code, KeyboardEventType::EVENT_KEY_RELEASED);
        }

    public:
        const KeyCode key;
        const KeyboardEventType type;
    };
} // namespace be
