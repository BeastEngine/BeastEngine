#pragma once
#include "Beast/Events/Events.h"
#include "Beast/Input/InputCodes.h"
#include "Beast/Windows/IWindow.h"
#include "Beast/BeastEngine.h"

#include <Beast/Common/Types.h>
#include <Beast/Common/Helpers.h>
#include <Beast/Math/Types.h>

namespace be
{
    /******************************************************************************/
    /********** CREATED FOR TESTING PURPOSES. THIS IS SUBJECT TO CHANGE **********/
    namespace
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

            void SetWheelScrollThreshold(uint16 threshold) noexcept
            {
                m_scrollThreshold = threshold;
            }

            void SetWheelScrolledListener(MouseWheelScrolledListener listener) noexcept
            {
                m_mouseScrolledListener = listener;
            }

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

            const auto& GetMousePosition() const noexcept
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
    } // namespace
    /******************************************************************************/
    /******************************************************************************/

    /**
     * @brief Abstract class representing single instace of the Application.
     * Its purpose is to initialize and handle the engine.
     */
    class AApplication
    {
    public:
        BE_IMPLEMENT_ADDITIONAL_CONSTRUCTORS_DELETED(AApplication)

        /**
         * @brief Creates instance of the AApplication class initializing engine instance with passed EngineConfig.
         * 
         * @param engineConfig - Configuration which will be used to create the engine instance
         * @param mainWindowDescriptor - Descriptor which will be used to create the main window
         */
        AApplication(EngineConfig engineConfig, const WindowDescriptor& mainWindowDescriptor);
        virtual ~AApplication() = default;

        /**
         * @brief Starts the application.
         * Should contain all the run-time code of the app.
         */
        virtual void Run() = 0;

        /**
         * @brief Returns engine.
         * It should be used by children to access the engine's functionalities.
         * 
         * @return
         */
        BeastEngine& GetEngine()
        {
            return *m_engine;
        }

    protected:
        Unique<Mouse> m_mouse = nullptr;
        Unique<Keyboard> m_keyboard = nullptr;
        Unique<IWindow> m_window = nullptr;

    private:
        Unique<BeastEngine> m_engine;
    };
} // namespace be
