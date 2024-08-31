#pragma once
#include "Beast/Loggers/Logger.h"
#include "Beast/Windows/Window.h"

#include "Beast/Common/Helpers.h"
#include "Beast/Common/Types.h"

#include "Beast/Graphics/IGraphics.h"

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
    };

    /**
     * @brief Main class of the engine. Represents its single instance.
     * It is an entry point for any engine related code.
     */
    class BeastEngine final
    {
    public:
        BE_IMPLEMENT_ADDITIONAL_CONSTRUCTORS_DELETED(BeastEngine)

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
        Unique<Window> CreateNewWindow(const WindowDescriptor& descriptor) const;

        /**
         * @brief Creates an instance of the Graphics Adapter implementation of the given \p api for given \p window.
         * The adapter can be used to create rendering resources and issue rendering commands.
         * 
         * @param api - Rendering API that should be used underneath. @see graphics::RenderingApi
         * @param window - The windows instance that will be used as a target for rendering
         * 
         * @return The new, unique instance of the adapter
         */
        Unique<graphics::IGraphics> CreateGraphics(const Window& window, graphics::RenderingApi api = graphics::RenderingApi::D3D11) const;

    private:
        void SetLogger(EngineConfig& config);
    };
} // namespace be
