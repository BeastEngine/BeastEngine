#include <Beast/EntryPoint.h>
#include <Beast/BeastEngine.h>
#include <Beast/Loggers/LoggersFactories.h>
#include <Beast/Common/Types.h>
#include <Beast/Common/Utils/Hasher.h>
#include <Beast/Graphics/2DRenderer.h>
#include <Beast/Graphics/Camera2D.h>

#include <iostream>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <spdlog/spdlog.h>

#include <shared_mutex>
#include <mutex>
#include <unordered_map>
#include <typeinfo>
#include <typeindex>

#include <box2d/box2d.h>

constexpr be::Id TEXTURE_ID = be::Id("Path/To/My/Texture");

struct Player
{
    b2Vec2 position;
    be::graphics::Sprite sprite;
    b2BodyId rigidBody;
};

struct Wall
{
    b2Vec2 position;
    be::graphics::Sprite sprite;
    b2BodyId rigidBody;
};

class BasicApplication final : public be::AApplication
{
public:
    BasicApplication(be::EngineConfig engineConfig, const be::WindowDescriptor& windowDescriptor)
        : be::AApplication(std::move(engineConfig), windowDescriptor), m_logger(be::ConsoleLogger::Create("client_console_logger"))
    {
        //m_mouse->SetWheelScrolledListener(OnWheelScrolled());
    }

    void Run() override
    {
        GetEngine().PrintInfo();

        b2WorldDef worldDef = b2DefaultWorldDef();
        worldDef.gravity = {0.0f, 0.0f};

        b2WorldId world = b2CreateWorld(&worldDef);

        Player player{
            .position = {0.0f, 0.0f},
            .sprite = {.color = {1.0f, 0.0f, 0.0f, 1.0f}},
        };

        b2BodyDef playerDef = b2DefaultBodyDef();
        playerDef.type = b2_dynamicBody;
        playerDef.position = player.position;

        player.rigidBody = b2CreateBody(world, &playerDef);
        b2Polygon playerBox = b2MakeBox(25.0f, 25.0f);

        b2ShapeDef playerShapeDef = b2DefaultShapeDef();
        b2CreatePolygonShape(player.rigidBody, &playerShapeDef, &playerBox);

        Wall wall{
            .position = {200.0f, 0.0f},
            .sprite = {.color = {0.01f, 0.2f, 0.89f, 1.0f}},
        };

        b2BodyDef wallDef = b2DefaultBodyDef();
        wallDef.type = b2_staticBody;
        wallDef.position = wall.position;

        wall.rigidBody = b2CreateBody(world, &wallDef);
        b2Polygon wallBox = b2MakeBox(25.0f, 25.0f);

        b2ShapeDef wallShapeDef = b2DefaultShapeDef();
        b2CreatePolygonShape(wall.rigidBody, &wallShapeDef, &wallBox);

        be::graphics::Renderer2D renderer(GetEngine().CreateGraphics(*m_window), 2);

        static constexpr float timeStep = 1.0f / 60.0f;
        static constexpr int subStepCount = 4;

        const auto& input = m_window->GetInputHandler();
        const auto& windowDimensions = m_window->GetDimensions();
        be::graphics::Camera2D camera{
            be::graphics::Viewport{windowDimensions},
            {0.0f, 0.0f},
        };

        while (true)
        {
            m_window->ProcessInput();
            renderer.StartFrame();

            if (input.IsKeyDown(be::KeyCode::D))
            {
                b2Body_SetLinearVelocity(player.rigidBody, {10.0f, 0.0f});
            }

            b2World_Step(world, timeStep, subStepCount);
            player.position = b2Body_GetPosition(player.rigidBody);

            renderer.AddSprite({player.position.x, player.position.y}, player.sprite);
            renderer.AddSprite({wall.position.x, wall.position.y}, wall.sprite);
            if (input.IsKeyPressed(be::KeyCode::Escape) || m_window->ShouldClose())
            {
                break;
            }
            renderer.EndFrame(camera);
        }
    }

private:
    const be::Shared<be::Logger> m_logger = nullptr;
};

be::Unique<be::AApplication> be::CreateApplication(WindowHandleInstance windowHandleInstance)
{
    // Configure engine
    auto config = be::EngineConfig();

    // Configure window
    be::WindowDescriptor windowDescriptor(std::move(windowHandleInstance));
    windowDescriptor.style = WindowStyle::WINDOW_DEFUALT;

    return be::MakeUnique<BasicApplication>(std::move(config), windowDescriptor);
}
