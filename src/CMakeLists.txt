set(BEAST_INCLUDE_DIR "${BeastEngine_SOURCE_DIR}/include")
set(BEAST_HEADERS_DIR "${BEAST_INCLUDE_DIR}/BeastEngine")
set(BEAST_SRC_DIR "${BeastEngine_SOURCE_DIR}/src")

# Set the variable and the global property to contain the name of the library for the BeastEngine project
set(BEAST_TARGET_NAME beastengine)
set_property(GLOBAL PROPERTY BEAST_LIB_TARGET_NAME "${BEAST_TARGET_NAME}")

# Set variable containing all of the library's header files
set(
	BEAST_HEADERS_LIST
	"${BEAST_HEADERS_DIR}/beastengine.h"
)

# Set variable containing all of the library's source files
set(
	BEAST_SRC_LIST
	"${BEAST_SRC_DIR}/beastengine.cpp"
)

add_library("${BEAST_TARGET_NAME}" SHARED "${BEAST_SRC_LIST}" "${BEAST_HEADERS_LIST}")
target_include_directories("${BEAST_TARGET_NAME}" PUBLIC "${BEAST_INCLUDE_DIR}")
target_compile_features("${BEAST_TARGET_NAME}" PUBLIC cxx_std_17)

GROUP_FILES("${BEAST_HEADERS_LIST}" "${BEAST_HEADERS_DIR}")
GROUP_FILES("${BEAST_SRC_LIST}" "${BEAST_SRC_DIR}")

install(TARGETS "${BEAST_TARGET_NAME}" DESTINATION "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")