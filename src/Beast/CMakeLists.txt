add_subdirectory(Common)
add_subdirectory(Graphics/D3D11/Shaders)

if(WIN32)
  list(APPEND BEAST_SOURCES "beastengine.dll.manifest")
endif()

message(STATUS "${BEAST_SOURCES}")
message(STATUS "${BEAST_API_FILES}")
add_library(
  BeastCore STATIC
  "${BEAST_SOURCES}"
  "${BEAST_API_FILES}"
  "Application.hpp"
  "BeastEngine.hpp"
  "DataStructures.hpp"
  "Debug.hpp"
  "Input/Events/Events.hpp"
  "Input/Events/KeyboardEvents.hpp"
  "Input/Events/MouseEvents.hpp"
  "Input/InputCodes.hpp"
  "Loggers/Logger.hpp"
  "Loggers/LoggersFactories.hpp"
  "Loggers/StaticLogger.hpp"
  "PlatformSetup.hpp"
  "Versions.hpp"
  "Windows/Window.hpp"
  "EntryPoint.hpp"
  "Math/Types.hpp"
  "Graphics/D3D11/Debug.hpp"
  "Graphics/D3D11/Buffer.hpp"
  "Graphics/D3D11/D3D11.hpp"
  "Graphics/IGraphics.hpp"
  "Graphics/GraphicsFactory.hpp"
  "Graphics/GraphicsFactory.cpp"
  "Graphics/2DRenderer.hpp"
  "Graphics/2DRenderer.cpp"
  "Graphics/D3D11/Shader.hpp"
  "Graphics/Types.hpp"
  "Logging.hpp"
  "Application.cpp"
  "BeastEngine.cpp"
  "Loggers/Logger.cpp"
  "Loggers/LoggersFactories.cpp"
  "Windows/Win32/Win32Window.cpp"
  "Windows/Win32/Win32Window.hpp"
  "Graphics/D3D11/Graphics.hpp"
  "Graphics/D3D11/Graphics.cpp"
  "Graphics/D3D11/Utils.hpp"
  "Graphics/D3D11/Utils.cpp"
  "Graphics/D3D11/RenderTarget.hpp"
  "Windows/Window.cpp"
  "Input/Input.hpp"
)
target_include_directories(BeastCore PUBLIC "${BeastEngine_SOURCE_DIR}/src")
be_set_compiler_options(BeastCore)

find_package(spdlog REQUIRED)
find_package(fmt REQUIRED)
find_package(glm REQUIRED)
target_link_libraries(BeastCore PUBLIC BE::common spdlog::spdlog fmt::fmt glm::glm)
add_dependencies(BeastCore BeCoreShaders)

set(BEAST_DEBUGGING_DEFINES $<$<CONFIG:Debug>:BE_DEBUG>)
if(${BE_ENABLE_DEBUG_INFO})
  list(APPEND BEAST_DEBUGGING_DEFINES BE_DEBUGGING_INFO_ENABLED)
endif()

if(${BE_ENABLE_ASSERTIONS})
  list(APPEND BEAST_DEBUGGING_DEFINES BE_ASSERTIONS_ENABLED)
endif()

target_compile_definitions(BeastCore PRIVATE ${BEAST_DEBUGGING_DEFINES})

# TODO: Remove when spdlog depends on at least fmt/10.1.1
target_compile_definitions(BeastCore PUBLIC _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING)

if(WIN32)
  target_compile_definitions(BeastCore PUBLIC BE_PLATFORM_WINDOWS)
  target_link_libraries(BeastCore PUBLIC d3d11.lib Rpcrt4.lib D3dcompiler.lib)
endif()

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/Versions.hpp.in" "${CMAKE_CURRENT_SOURCE_DIR}/Versions.hpp")

add_library(BE::core ALIAS BeastCore)
