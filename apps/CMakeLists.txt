add_executable(${BEAST_EXE_TARGET_NAME} sandbox.cpp)
if (WIN32)
	# Create WinApi application while using the system console
	target_link_options(
		${BEAST_EXE_TARGET_NAME}
		PRIVATE
			/ENTRY:wWinMainCRTStartup /SUBSYSTEM:CONSOLE
	)
endif()

target_link_libraries(${BEAST_EXE_TARGET_NAME} PRIVATE ${BEAST_LIB_TARGET_NAME})

install(TARGETS ${BEAST_EXE_TARGET_NAME} DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
