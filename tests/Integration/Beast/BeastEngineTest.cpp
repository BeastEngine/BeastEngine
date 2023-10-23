#include "Integration/Beast/BeastEngineTest.h"

#include <Beast/BeastEngine.h>
#include <Beast/Graphics/GraphicsFactory.h>

namespace be::tests::integration
{
    TEST_F(BeastEngineTest, CreateMainWindowWillReturnValidWindowWhenNoWindowFactoryImplementationProvided)
    {
        WindowHandleInstanceType hwndInstance = nullptr;
        auto windowParams = WindowDescriptor(hwndInstance);
        windowParams.dimensions = {0, 0};

        const auto sut = BeastEngine(EngineConfig());
        const auto result = sut.CreateNewWindow(windowParams);
        ASSERT_NE(nullptr, result);
    }

    TEST_F(BeastEngineTest, CreateGraphicsWillUseProvidedFactory)
    {
        class Context final : public graphics::IContext
        {
        public:
            Context(uint32 id)
                : m_id(id)
            {
            }

        private:
            void Clear(const graphics::Color&) const noexcept override{};
            void Present() const noexcept override{};

        public:
            uint32 m_id;
        };

        class Device final : public graphics::IDevice
        {
        public:
            Device(uint32 id)
                : m_id(id)
            {}

        private:
            graphics::VertexBuffer CreateVertexBuffer() override
            {
                return {};
            }

        public:
            uint32 m_id;
        };

        class GraphicsFactory final : public graphics::IGraphicsFactory
        {
        public:
            GraphicsFactory(uint32 contextId, uint32 deviceId)
                : m_contextId(contextId), m_deviceId(deviceId)
            {
            }

        private:
            Unique<graphics::Graphics> Create(graphics::RenderingApi, const IWindow&) const override
            {
                return MakeUnique<graphics::Graphics>(MakeUnique<Context>(m_contextId), MakeUnique<Device>(m_deviceId));
            }

        private:
            uint32 m_contextId;
            uint32 m_deviceId;
        };

        uint32 expectedContextId = 1;
        uint32 expectedDeviceId = 2;

        auto factory = MakeUnique<GraphicsFactory>(expectedContextId, expectedDeviceId);

        EngineConfig config{
            .graphicsFactory = std::move(factory),
        };

        const BeastEngine sut(std::move(config));
        
        WindowHandleInstanceType hwndInstance = nullptr;
        auto windowParams = WindowDescriptor(hwndInstance);
        windowParams.dimensions = {0, 0};

        const auto window = sut.CreateNewWindow(windowParams);
        auto graphics = sut.CreateGraphics(graphics::RenderingApi::D3D11, *window);

        auto& context = graphics->Context();
        auto& device = graphics->Device();

        ASSERT_EQ(expectedContextId, static_cast<Context*>(&context)->m_id);
        ASSERT_EQ(expectedDeviceId, static_cast<Device*>(&device)->m_id);
    }
} // namespace be::tests::integration
