set(
    BEAST_API_LIST
    "Types.h"
)

set(
    BE_MATH_SRC_LIST
)

add_library(BeastMath INTERFACE "${BE_MATH_SRC_LIST}" "${BEAST_API_LIST}")
target_include_directories(BeastMath INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_features(BeastMath INTERFACE cxx_std_20)

find_package(glm REQUIRED)
target_link_libraries(
    BeastMath
    INTERFACE
        glm::glm
)

add_library(BE::math ALIAS BeastMath)
