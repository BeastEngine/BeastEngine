#pragma once
#include "BeastEngine/Core/Types.h"
#include "BeastEngine/Core/Helpers.h"
#include "BeastEngine/Core/Loggers/Logger.h"
#include "BeastEngine/Core/Events/Events.h"
#include "BeastEngine/Core/Windows/IWindow.h"
#include "BeastEngine/Core/Windows/IWindowFactory.h"

namespace be
{
    /**
     * Contains configuration for the BeastEngine class.
     */
    struct EngineConfig
    {
        /**
         * Logger that will be used by engine subsystems.
         * If set to nullptr, engine will create ConsoleLogger by default
         */
        SharedPtr<Logger> logger = nullptr;
        
        /**
         * IWindowFactory implementation that will be used to create windows by the engine.
         * If set to nullptr, the default engine's implementation will be used
         */
        UniquePtr<IWindowFactory> windowFactory = nullptr;
    };

    class BeastEngine final
    {
    public:
        BE_IMPLEMENT_ADDITIONAL_CONSTRUCTORS_DELETED(BeastEngine)
        
        /**
         * Initializes all engine subsystems using passed configuration.
         * 
         * @param config - Engine's subsystems configuration struct
         */
        BeastEngine(EngineConfig config);
        ~BeastEngine() = default;

        /**
         * Prints information about current engine's version into the stdout.
         */
        void PrintInfo() const;

        /**
         * Creates and returns window using IWindowFactory implementation passed inside EngineConfig.
         * 
         * @param descriptor
         * @return UniquePointer to the created window
         */
        UniquePtr<IWindow> CreateMainWindow(const WindowDescriptor& descriptor) const;

    private:
        void SetLogger() const;
        void SetWindowFactory();

    private:
        EngineConfig m_config;
    };
} // namespace be
