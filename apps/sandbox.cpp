#include <BeastEngine/EntryPoint.h>

class BasicApplication final : public be::AApplication
{
public:
    BasicApplication(be::EngineConfig engineConfig)
        : be::AApplication(std::move(engineConfig))
    {
    }

    virtual void Run() override
    {
        m_engine->PrintInfo();
    }
};

be::UniquePtr<be::AApplication> be::CreateApplication()
{
    return be::CreateUniquePtr<BasicApplication>(be::EngineConfig());
}
