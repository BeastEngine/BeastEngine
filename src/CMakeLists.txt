include("${BEAST_CONFIG_PATH}/beastengine/config.cmake")

add_library("${BEAST_LIB_TARGET_NAME}" SHARED "${BEAST_SRC_LIST}" "${BEAST_HEADERS_LIST}")
target_include_directories("${BEAST_LIB_TARGET_NAME}" PUBLIC "${BEAST_INCLUDE_DIR}")
target_compile_features("${BEAST_LIB_TARGET_NAME}" PUBLIC cxx_std_17)

# Add linking to libraries from Conan
conan_target_link_libraries("${BEAST_LIB_TARGET_NAME}")

# Group files into proper folders - for IDE
GROUP_FILES("${BEAST_HEADERS_LIST}" "${BEAST_INCLUDE_DIR}")
GROUP_FILES("${BEAST_SRC_LIST}" "${BEAST_SRC_DIR}")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    # target_compile_options("${BEAST_LIB_TARGET_NAME}" PRIVATE /EHsc /MTd /W2 /c)
    # Set the DLLEXPORT variable to export symbols
    # target_compile_definitions("${BEAST_LIB_TARGET_NAME}" PRIVATE WIN_EXPORT)
endif()

install(TARGETS "${BEAST_LIB_TARGET_NAME}" DESTINATION "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")