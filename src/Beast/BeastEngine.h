#pragma once
#include "Beast/Loggers/Logger.h"
#include "Beast/Windows/IWindow.h"
#include "Beast/Windows/IWindowFactory.h"

#include <Beast/Common/Helpers.h>
#include <Beast/Common/Types.h>

namespace be
{
    /**
     * @brief Contains configuration of the BeastEngine class.
     */
    struct EngineConfig
    {
        /**
         * @brief Logger that will be used by engine subsystems.
         * If set to nullptr, engine will create ConsoleLogger by default.
         */
        Shared<Logger> logger = nullptr;

        /**
         * @brief IWindowFactory implementation that will be used to create windows by the engine.
         * If set to nullptr, the default engine's implementation will be used.
         */
        Unique<IWindowFactory> windowFactory = nullptr;
    };

    /**
     * @brief Main class of the engine. Represents its single instance.
     * It is an entry point for any engine related code.
     */
    class BeastEngine final
    {
    public:
        BE_IMPLEMENT_ADDITIONAL_CONSTRUCTORS_DELETED(BeastEngine)

        /**
         * @brief Initializes all engine subsystems using passed configuration.
         * 
         * @param config - Engine's subsystems configuration struct
         */
        BeastEngine(EngineConfig config);
        ~BeastEngine() = default;

        /**
         * @brief Prints information about current engine's version into the stdout.
         */
        void PrintInfo() const;

        /**
         * @brief Creates and returns new window using IWindowFactory implementation passed inside EngineConfig.
         * 
         * @param descriptor
         * @return UniquePointer to the created window
         */
        Unique<IWindow> CreateNewWindow(const WindowDescriptor& descriptor) const;

    private:
        void SetLogger(EngineConfig& config);
        void SetWindowFactory(EngineConfig& config);

    private:
        Unique<IWindowFactory> m_windowFactory = nullptr;
    };
} // namespace be
