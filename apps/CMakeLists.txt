# Set the variable and the global property to contain the name of the executable for the BeastEngine project
set(SANDBOX_TARGET_NAME sandbox)
set_property(GLOBAL PROPERTY BEAST_EXE_TARGET_NAME "${SANBOX_TARGET_NAME}")

# Get the name of the target of the BeastEngine library - this property should be set before. If not, throw error
get_property(IS_BEAST_LIB_TARGET_NAME_SET GLOBAL PROPERTY BEAST_LIB_TARGET_NAME SET)
if (NOT IS_BEAST_LIB_TARGET_NAME_SET)
	set (
		ERROR_MESSAGE
		"The 'BEAST_LIB_TARGET_NAME' global property must be set in order to define executable.\nDid you run configuration for the library?"
	)
	message(FATAL_ERROR "${ERROR_MESSAGE}")
endif()
get_property(BEAST_LIB_TARGET_NAME GLOBAL PROPERTY BEAST_LIB_TARGET_NAME)

add_executable("${SANDBOX_TARGET_NAME}" sandbox.cpp)
target_link_libraries("${SANDBOX_TARGET_NAME}" PRIVATE "${BEAST_LIB_TARGET_NAME}")

install(TARGETS "${SANDBOX_TARGET_NAME}" DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")