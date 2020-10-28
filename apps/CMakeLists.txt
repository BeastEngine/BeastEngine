include("${PROJECT_CONFIG_PATH}/sandbox/config.cmake")

add_executable(${BEAST_EXE_TARGET_NAME} "${SANDBOX_SRC_LIST}" "${SANDBOX_HEADERS_LIST}")
if (WIN32)
	# Create WinApi application while using the system console
	target_link_options(
		${BEAST_EXE_TARGET_NAME}
		PRIVATE
			/ENTRY:wWinMainCRTStartup /SUBSYSTEM:CONSOLE
	)
endif()

target_link_libraries(${BEAST_EXE_TARGET_NAME} PRIVATE ${BEAST_LIB_TARGET_NAME})

# Group files into proper folders - for IDE
GROUP_FILES("${SANDBOX_HEADERS_LIST}" "${SANDBOX_INCLUDE_DIR}")
GROUP_FILES("${SANDBOX_SRC_LIST}" "${SANDBOX_SRC_DIR}")

install(TARGETS ${BEAST_EXE_TARGET_NAME} DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
