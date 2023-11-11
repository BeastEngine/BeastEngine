add_subdirectory(Common)
add_subdirectory(Graphics/D3D11/Shaders)

set(
    BEAST_API_FILES
    "Application.h"
    "Assertions.h"
    "BeastEngine.h"
    "DataStructures.h"
    "Debugging.h"
    "Events/Events.h"
    "Events/KeyboardEvents.h"
    "Events/MouseEvents.h"
    "Input/InputCodes.h"
    "Loggers/Logger.h"
    "Loggers/LoggersFactories.h"
    "Loggers/StaticLogger.h"
    "PlatformSetup.h"
    "Versions.h"
    "Windows/AWindow.h"
    "Windows/IWindow.h"
    "Windows/IWindowFactory.h"
    "EntryPoint.h"
    "Math/Types.h"
  "Ecs/AccessList.h" "Ecs/Types.h" "Ecs/View.h" "Ecs/World.h"  "Ecs/SystemsScheduler.h" "Ecs/Ecs.h")

set(
    BEAST_SOURCES
    "Application.cpp"
    "BeastEngine.cpp"
    "Loggers/Logger.cpp"
    "Loggers/LoggersFactories.cpp"
    "Windows/Win32/Win32Window.cpp"
    "Windows/Win32/Win32Window.h"
    "Windows/WindowFactory.cpp"
    "Windows/WindowFactory.h"
  "Ecs/SystemsScheduler.cpp")

if(WIN32)
    list(APPEND BEAST_SOURCES "beastengine.dll.manifest")
endif()

message(STATUS "${BEAST_SOURCES}")
message(STATUS "${BEAST_API_FILES}")
add_library(
    BeastCore
        STATIC
            "${BEAST_SOURCES}"
            "${BEAST_API_FILES}"
            "Graphics/D3D11/Context.h"
            "Graphics/D3D11/Context.cpp"
            
            "Graphics/D3D11/Asserts.h" "Graphics/D3D11/VertexBuffer.h" "Graphics/D3D11/D3D11.h"  "Graphics/Graphics.h" "Graphics/Graphics.cpp" "Graphics/D3D11/Device.h" "Graphics/D3D11/Device.cpp"  "Graphics/GraphicsFactory.h" "Graphics/GraphicsFactory.cpp" "Ecs/Components/Core.h" "Graphics/Renderer.h" "Graphics/Renderer.cpp" "Graphics/D3D11/VertexShader.h" "Graphics/Types.h" "Ecs/Components/Graphics.h" "Graphics/D3D11/PixelShader.h")
target_include_directories(
    BeastCore
        PUBLIC
            "${BeastEngine_SOURCE_DIR}/src"
)
be_set_compiler_options(BeastCore)

find_package(spdlog REQUIRED)
find_package(fmt REQUIRED)
find_package(entt REQUIRED)
find_package(glm REQUIRED)
target_link_libraries(
    BeastCore
    PUBLIC
        BE::common
        spdlog::spdlog
        fmt::fmt
        EnTT::EnTT
        glm::glm
)
add_dependencies(BeastCore BeCoreShaders)

set(BEAST_DEBUGGING_DEFINES)
if (${BE_ENABLE_DEBUG_INFO})
    list(APPEND BEAST_DEBUGGING_DEFINES BE_DEBUGGING_INFO_ENABLED)
endif()

if (${BE_ENABLE_ASSERTIONS})
    list(APPEND BEAST_DEBUGGING_DEFINES BE_ASSERTIONS_ENABLED)
endif()

target_compile_definitions(BeastCore PRIVATE ${BEAST_DEBUGGING_DEFINES})

if(WIN32)
    target_compile_definitions(BeastCore PUBLIC BE_PLATFORM_WINDOWS)
    target_link_libraries(
        BeastCore
        PUBLIC
            d3d11.lib
            Rpcrt4.lib
            D3dcompiler.lib
    )
endif()

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/Versions.h.in"
    "${CMAKE_CURRENT_SOURCE_DIR}/Versions.h"
)

add_library(BE::core ALIAS BeastCore)
