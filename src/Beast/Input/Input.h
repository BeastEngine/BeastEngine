#pragma once
#include "Beast/Input/InputCodes.h"
#include "Beast/Input/Events/Events.h"

#include "Beast/Common/Types.h"

#include <array>

namespace be
{
    namespace internals
    {
        struct Keyboard
        {
            std::array<uint8, be::ToUnderlying(KeyCode::SIZE)> states{0u};
        };

        struct Mouse
        {
            constexpr void Scroll(int16 scrollAmount)
            {
                currentScrollValue += scrollAmount;

                while (currentScrollValue >= scrollThreshold)
                {
                    currentScrollValue -= scrollThreshold;
                }

                while (currentScrollValue <= -scrollThreshold)
                {
                    currentScrollValue += scrollThreshold;
                }
            }

            std::array<uint8, be::ToUnderlying(KeyCode::SIZE)> states{0u};
            Vec2i coordinates;

            uint16 scrollThreshold = 120;
            int16 currentScrollValue = 0;
            WheelScrollDirection currentScrollDirection = WheelScrollDirection::SCROLL_NONE;
        };
    }; // namespace internals

    class Input
    {
    public:
        constexpr void OnKeyPressed(KeyCode key)
        {
            m_keyboard.states[ToUnderlying(key)] = KEY_PRESSED;
        }

        constexpr void OnKeyHeldDown(KeyCode key)
        {
            m_keyboard.states[ToUnderlying(key)] = KEY_HELD_DOWN;
        }

        constexpr void OnKeyReleased(KeyCode key)
        {
            m_keyboard.states[ToUnderlying(key)] = 0u;
        }

        constexpr bool IsKeyPressed(KeyCode keyCode) const noexcept
        {
            return m_keyboard.states[ToUnderlying(keyCode)] == KEY_PRESSED;
        }

        constexpr bool IsKeyHeldDown(KeyCode keyCode) const noexcept
        {
            return m_keyboard.states[ToUnderlying(keyCode)] == KEY_HELD_DOWN;
        }

        constexpr bool IsKeyDown(KeyCode keyCode) const noexcept
        {
            return IsKeyHeldDown(keyCode) || IsKeyPressed(keyCode);
        }

        constexpr void SetMouseWheelScrollThreshold(uint16 threshold) noexcept
        {
            m_mouse.scrollThreshold = threshold;
        }

        constexpr void OnMouseButtonPressed(MouseButtonCode button, Vec2i&& mousePosition)
        {
            m_mouse.states[ToUnderlying(button)] = KEY_PRESSED;
            m_mouse.coordinates = std::move(mousePosition);
        }

        constexpr void OnMouseButtonHeldDown(MouseButtonCode button)
        {
            m_mouse.states[ToUnderlying(button)] = KEY_HELD_DOWN;
        }

        constexpr void OnMouseButtonReleased(MouseButtonCode button, Vec2i&& mousePosition)
        {
            m_mouse.states[ToUnderlying(button)] = 0u;
            m_mouse.coordinates = std::move(mousePosition);
        }

        constexpr void OnMouseMoved(Vec2i&& mousePosition)
        {
            m_mouse.coordinates = std::move(mousePosition);
        }

        constexpr void OnMouseWheelScrolled(int16 scrollAmount, Vec2i&& mousePosition)
        {
            m_mouse.coordinates = std::move(mousePosition);
            m_mouse.Scroll(scrollAmount);
        }

        constexpr void ResetMouseWheel()
        {
            m_mouse.currentScrollDirection = WheelScrollDirection::SCROLL_NONE;
        }

        constexpr bool IsMouseButtonPressed(MouseButtonCode button) const
        {
            return m_mouse.states[ToUnderlying(button)] == KEY_PRESSED;
        }

        constexpr bool IsMouseButtonHeldDown(MouseButtonCode button) const
        {
            return m_mouse.states[ToUnderlying(button)] == KEY_HELD_DOWN;
        }

        constexpr const Vec2i& GetMousePosition() const
        {
            return m_mouse.coordinates;
        }

        constexpr const WheelScrollDirection GetMouseWheelScroll() const
        {
            return m_mouse.currentScrollDirection;
        }

    private:
        static constexpr uint8 KEY_PRESSED = 0b00000001;
        static constexpr uint8 KEY_HELD_DOWN = 0b00000010;

        internals::Keyboard m_keyboard;
        internals::Mouse m_mouse;
    };
} // namespace be
