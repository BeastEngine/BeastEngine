#pragma once
#include "BeastEngine/Core/Helpers.h"
#include "BeastEngine/Core/BeastEngine.h"

namespace be
{
    class AApplication
    {
    public:
        BE_IMPLEMENT_CONSTRUCTORS_DEFAULT(AApplication)

        /**
         * Creates instance of the AApplication class initializing engine instance with passed be::EngineConfig.
         * 
         * @param engineConfig
         */
        AApplication(EngineConfig engineConfig)
        {
            m_engine = CreateUniquePtr<BeastEngine>(std::move(engineConfig));
        }
        virtual ~AApplication() = default;

        /**
         * Starts the application.
         * Should contain all the run-time code of the app.
         * 
         */
        virtual void Run() = 0;

    protected:
        UniquePtr<BeastEngine> m_engine;
    };
} // namespace be