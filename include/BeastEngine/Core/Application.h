#pragma once
#include "BeastEngine/Core/Helpers.h"
#include "BeastEngine/Core/BeastEngine.h"
#include "BeastEngine/Core/Windows/IWindow.h"

namespace be
{
    /******************************************************************************/
    /********** CREATED FOR TESTING PURPOSES. THIS IS SUBJECT TO CHANGE **********/
    namespace internals
    {
        struct ButtonState
        {
            bool isPressed = false;
            bool isHeldDown = false;
        };

        class Mouse
        {
        public:
            Mouse(IWindow& window) noexcept;

            void SetWheelScrollThreshold(uint16 threshold) noexcept;
            void SetWheelScrolledListener(MouseWheelScrolledListener listener) noexcept;

            bool IsButtonPressed(MouseButtonCode buttonCode) const noexcept
            {
                if (const auto buttonState = m_buttonsStates.find(buttonCode); buttonState != m_buttonsStates.end())
                {
                    return buttonState->second.isPressed;
                }

                return false;
            }

            bool IsButtonHeldDown(MouseButtonCode buttonCode) const noexcept
            {
                if (const auto buttonState = m_buttonsStates.find(buttonCode); buttonState != m_buttonsStates.end())
                {
                    return buttonState->second.isHeldDown;
                }

                return false;
            }

            const auto& GetCoordinates() const noexcept
            {
                return m_coordinates;
            }

        private:
            MouseEventHandler GetEventHandler() noexcept;
            void ScrollWheel(int16 scrollAmount) noexcept;

        private:
            IntVec2 m_coordinates;
            std::unordered_map<MouseButtonCode, ButtonState> m_buttonsStates;

            uint16 m_scrollThreshold = 120;
            int16 m_currentScrollValue = 0;

            MouseWheelScrolledListener m_mouseScrolledListener;
        };

        class Keyboard
        {
        public:
            Keyboard(IWindow& window) noexcept;

            bool IsKeyPressed(KeyCode keyCode) const noexcept
            {
                if (const auto buttonState = m_buttonsStates.find(keyCode); buttonState != m_buttonsStates.end())
                {
                    return buttonState->second.isPressed;
                }

                return false;
            }

            bool IsKeyHeldDown(KeyCode buttonCode) const noexcept
            {
                if (const auto buttonState = m_buttonsStates.find(buttonCode); buttonState != m_buttonsStates.end())
                {
                    return buttonState->second.isHeldDown;
                }

                return false;
            }

            bool IsKeyDown(KeyCode buttonCode) const noexcept
            {
                if (const auto buttonState = m_buttonsStates.find(buttonCode); buttonState != m_buttonsStates.end())
                {
                    return buttonState->second.isHeldDown || buttonState->second.isPressed;
                }

                return false;
            }

        private:
            KeyboardEventHandler GetEventHandler() noexcept;

        private:
            std::unordered_map<KeyCode, ButtonState> m_buttonsStates;
        };
    } // namespace internals
    /******************************************************************************/
    /******************************************************************************/

    class AApplication
    {
    public:
        BE_IMPLEMENT_CONSTRUCTORS_DEFAULT(AApplication)

        /**
         * Creates instance of the AApplication class initializing engine instance with passed be::EngineConfig.
         * 
         * @param engineConfig
         * @param mainWindowDescriptor
         */
        AApplication(EngineConfig engineConfig, const WindowDescriptor& mainWindowDescriptor);
        virtual ~AApplication() = default;

        /**
         * Starts the application.
         * Should contain all the run-time code of the app.
         */
        virtual void Run() = 0;

    protected:
        UniquePtr<internals::Mouse> m_mouse = nullptr;
        UniquePtr<internals::Keyboard> m_keyboard = nullptr;
        UniquePtr<IWindow> m_window = nullptr;

    protected:
        UniquePtr<BeastEngine> m_engine;
    };
} // namespace be