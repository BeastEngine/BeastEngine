SET(SANDBOX_TARGET_NAME Sandbox)

set(SANDBOX_INCLUDE_DIR "${${PROJECT_MAIN_NAME}_SOURCE_DIR}")
set(SANDBOX_SRC_DIR "${${PROJECT_MAIN_NAME}_SOURCE_DIR}")

set(SANDBOX_HEADERS_PREFIX "${SANDBOX_INCLUDE_DIR}/${SANDBOX_TARGET_NAME}/apps")
set(SANDBOX_SRC_PREFIX "${SANDBOX_SRC_DIR}/${SANDBOX_TARGET_NAME}/apps")

set(
    SANDBOX_HEADERS_LIST
  )

set(
    SANDBOX_SRC_LIST
    "Sandbox.cpp"
 "Ideas.cpp")

add_executable(${SANDBOX_TARGET_NAME} "${SANDBOX_SRC_LIST}" "${SANDBOX_HEADERS_LIST}")
target_link_libraries(${SANDBOX_TARGET_NAME} PRIVATE BE::core)

be_set_compiler_options(${SANDBOX_TARGET_NAME})

if (WIN32)
    # Create WinApi application while using the system console
    target_link_options(
        ${SANDBOX_TARGET_NAME}
        PRIVATE
            /ENTRY:wWinMainCRTStartup /SUBSYSTEM:CONSOLE
    )
endif()

# # Group files into proper folders - for IDE
# be_group_files("${SANDBOX_HEADERS_LIST}" "${SANDBOX_INCLUDE_DIR}")
# be_group_files("${SANDBOX_SRC_LIST}" "${SANDBOX_SRC_DIR}")
