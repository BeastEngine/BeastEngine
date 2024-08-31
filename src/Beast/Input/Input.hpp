#pragma once
#include "Beast/Input/InputCodes.hpp"
#include "Beast/Input/Events/Events.hpp"

#include "Beast/Common/Types.hpp"

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
                    wheelEvents.emplace_back(WheelScrollDirection::SCROLL_UP);
                }

                while (currentScrollValue <= -scrollThreshold)
                {
                    currentScrollValue += scrollThreshold;
                    wheelEvents.emplace_back(WheelScrollDirection::SCROLL_DOWN);
                }
            }

            std::array<uint8, be::ToUnderlying(KeyCode::SIZE)> states{0u};
            std::vector<MouseWheelScrolledEvent> wheelEvents{};
            Vec2i coordinates = {0, 0};

            uint16 scrollThreshold = 240;
            int16 currentScrollValue = 0;
        };
    }; // namespace internals

    class Input
    {
    public:
        constexpr void OnKeyPressed(KeyCode key) noexcept
        {
            m_keyboard.states[ToUnderlying(key)] = KEY_PRESSED;
        }

        constexpr void OnKeyHeldDown(KeyCode key) noexcept
        {
            m_keyboard.states[ToUnderlying(key)] = KEY_HELD_DOWN;
        }

        constexpr void OnKeyReleased(KeyCode key) noexcept
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

        constexpr std::vector<MouseWheelScrolledEvent> PollMouseWheelEvents() const noexcept
        {
            std::vector<MouseWheelScrolledEvent> oldEvents{};
            oldEvents.swap(m_mouse.wheelEvents);

            return oldEvents;
        }

        constexpr void SetMouseWheelScrollThreshold(uint16 threshold) noexcept
        {
            m_mouse.scrollThreshold = threshold;
        }

        constexpr void OnMouseButtonPressed(MouseButtonCode button, Vec2i&& mousePosition) noexcept
        {
            m_mouse.states[ToUnderlying(button)] = KEY_PRESSED;
            m_mouse.coordinates = std::move(mousePosition);
        }

        constexpr void OnMouseButtonHeldDown(MouseButtonCode button) noexcept
        {
            m_mouse.states[ToUnderlying(button)] = KEY_HELD_DOWN;
        }

        constexpr void OnMouseButtonReleased(MouseButtonCode button, Vec2i&& mousePosition) noexcept
        {
            m_mouse.states[ToUnderlying(button)] = 0u;
            m_mouse.coordinates = std::move(mousePosition);
        }

        constexpr void OnMouseMoved(Vec2i&& mousePosition) noexcept
        {
            m_mouse.coordinates = std::move(mousePosition);
        }

        constexpr void OnMouseWheelScrolled(int16 scrollAmount) noexcept
        {
            m_mouse.Scroll(scrollAmount);
        }

        constexpr bool IsMouseButtonPressed(MouseButtonCode button) const noexcept
        {
            return m_mouse.states[ToUnderlying(button)] == KEY_PRESSED;
        }

        constexpr bool IsMouseButtonHeldDown(MouseButtonCode button) const noexcept
        {
            return m_mouse.states[ToUnderlying(button)] == KEY_HELD_DOWN;
        }

        constexpr bool IsMouseButtonDown(MouseButtonCode button) const noexcept
        {
            return IsMouseButtonPressed(button) || IsMouseButtonHeldDown(button);
        }

        constexpr const Vec2i& GetMousePosition() const noexcept
        {
            return m_mouse.coordinates;
        }

        constexpr const int16 GetMouseWheelDelta() const noexcept
        {
            return m_mouse.currentScrollValue;
        }

    private:
        static constexpr uint8 KEY_PRESSED = 0b00000001;
        static constexpr uint8 KEY_HELD_DOWN = 0b00000010;

        internals::Keyboard m_keyboard;
        mutable internals::Mouse m_mouse;
    };
} // namespace be
