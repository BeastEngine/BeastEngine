#pragma once
#include "Beast/Windows/Window.hpp"
#include "Beast/BeastEngine.hpp"

#include "Beast/Common/Types.hpp"
#include "Beast/Common/Helpers.hpp"
#include "Beast/Math/Types.hpp"

namespace be
{
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
        void Start();

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
        virtual void Run() = 0;

    protected:
        Unique<Window> m_window = nullptr;

    private:
        Unique<BeastEngine> m_engine;
    };
} // namespace be
