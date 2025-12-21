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
  "Application.h"
  "BeastEngine.h"
  "DataStructures.h"
  "Debug.h"
  "Input/Events/Events.h"
  "Input/Events/KeyboardEvents.h"
  "Input/Events/MouseEvents.h"
  "Input/InputCodes.h"
  "Loggers/Logger.h"
  "Loggers/LoggersFactories.h"
  "Loggers/StaticLogger.h"
  "PlatformSetup.h"
  "Versions.h"
  "Windows/Window.h"
  "EntryPoint.h"
  "Math/Types.h"
  "Graphics/D3D11/Debug.h"
  "Graphics/D3D11/Buffer.h"
  "Graphics/D3D11/D3D11.h"
  "Graphics/IGraphics.h"
  "Graphics/GraphicsFactory.h"
  "Graphics/GraphicsFactory.cpp"
  "Graphics/2DRenderer.h"
  "Graphics/2DRenderer.cpp"
  "Graphics/D3D11/Shader.h"
  "Graphics/Types.h"
  "Logging.h"
  "Application.cpp"
  "BeastEngine.cpp"
  "Loggers/Logger.cpp"
  "Loggers/LoggersFactories.cpp"
  "Windows/Win32/Win32Window.cpp"
  "Windows/Win32/Win32Window.h"
  "Graphics/D3D11/Graphics.h"
  "Graphics/D3D11/Graphics.cpp"
  "Graphics/D3D11/Utils.h"
  "Graphics/D3D11/Utils.cpp"
  "Graphics/D3D11/RenderTarget.h"
  "Windows/Window.cpp"
  "Input/Input.h"
  "Graphics/Camera2D.h"
  "Graphics/Camera2D.cpp"
  "Physics/World.h"
  "Physics/Types.h"
  "Physics/World.cpp"
  "Common/Helpers.h"
  "Common/Types.h"
  "Common/Filesystem/Types.h"
  "Common/Filesystem/ResourcesManager.h"
  "Common/Filesystem/ResourcesManager.cpp"
  "Common/UUIDGenerator.h"
  "Common/Utils/Hasher.h"
  "Common/Utils/Xxh64.h"
  "Common/Id.h"
  "Common/AllocatorAwareId.h"
  "Common/IdAllocator.h"
  "Common/IdAllocator.cpp"
  "Common/IdAllocatorsFactory.h"
  "Common/IdAllocatorsFactory.cpp"
  "Graphics/ImageLoader.h"
  "Graphics/ImageLoader.cpp"
  "Common/Result.h"
  "Graphics/Pipeline.h"
)
target_include_directories(BeastCore PUBLIC "${BeastEngine_SOURCE_DIR}/src")
be_set_compiler_options(BeastCore)

find_package(spdlog REQUIRED)
find_package(fmt REQUIRED)
find_package(glm REQUIRED)
find_package(xxHash REQUIRED)
find_package(stduuid REQUIRED)
target_link_libraries(
  BeastCore
  PUBLIC spdlog::spdlog
         fmt::fmt
         glm::glm
         box2d::box2d
         xxHash::xxhash
         stduuid::stduuid
         stb
)
add_dependencies(BeastCore BeCoreShaders)

set(BEAST_DEBUGGING_DEFINES $<$<CONFIG:Debug>:BE_DEBUG>)

if(${BE_ENABLE_DEBUG_INFO})
  list(APPEND BEAST_DEBUGGING_DEFINES BE_DEBUGGING_INFO_ENABLED)
endif()

if(${BE_ENABLE_ASSERTIONS})
  list(APPEND BEAST_DEBUGGING_DEFINES BE_ASSERTIONS_ENABLED)
endif()

target_compile_definitions(BeastCore PUBLIC ${BEAST_DEBUGGING_DEFINES})

# TODO: Remove when spdlog depends on at least fmt/10.1.1
target_compile_definitions(BeastCore PUBLIC _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING)

if(WIN32)
  target_compile_definitions(BeastCore PUBLIC BE_PLATFORM_WINDOWS)
  target_link_libraries(BeastCore PUBLIC d3d11.lib Rpcrt4.lib D3dcompiler.lib)
endif()

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/Versions.h.in" "${CMAKE_CURRENT_SOURCE_DIR}/Versions.h")

add_library(BE::core ALIAS BeastCore)

add_custom_command(
  TARGET BeastCore
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${BE_DATA_BASE_DIR} ${BE_DATA_POST_BUILD_DIR} DEPENDS
          ${BE_DATA_POST_BUILD_DIR}
  COMMENT "Copying resources folder from ${BE_DATA_BASE_DIR} to ${BE_DATA_POST_BUILD_DIR}"
)
