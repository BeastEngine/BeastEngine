set(LAB_INTEGRATION_INCLUDE_DIR "${BeastEngine_SOURCE_DIR}/tests")
set(LAB_INTEGRATION_SRC_DIR "${BeastEngine_SOURCE_DIR}/tests")

set(
	LAB_INTEGRATION_HEADERS_LIST
	"${LAB_INTEGRATION_INCLUDE_DIR}/Integration/Core/Windows/Windows/WindowsMouseEventsTest.h"
    "${LAB_INTEGRATION_INCLUDE_DIR}/Integration/Core/Windows/Windows/WindowsKeyboardEventsTest.h"
    "${LAB_INTEGRATION_INCLUDE_DIR}/Integration/Core/Windows/Windows/WinTestSetup.h"
    "${LAB_INTEGRATION_INCLUDE_DIR}/TestSetup.h"
    "${LAB_INTEGRATION_INCLUDE_DIR}/TestCase.h"
)
set(
	LAB_INTEGRATION_SRC_LIST
	"${LAB_INTEGRATION_SRC_DIR}/lab_integration.cpp"
    "${LAB_INTEGRATION_SRC_DIR}/Integration/Core/Windows/Windows/WindowsMouseEventsTest.cpp"
    "${LAB_INTEGRATION_SRC_DIR}/Integration/Core/Windows/Windows/WindowsKeyboardEventsTest.cpp"
)

file(
	WRITE
	"C:/Users/Tomek/Documents/Projects/C++/BeastEngine/cmake/config/lab/lab_vars.txt"
	"\$\{LAB_INTEGRATION_INCLUDE_DIR\}=${LAB_INTEGRATION_INCLUDE_DIR}\n"
	"\$\{LAB_INTEGRATION_SRC_DIR\}=${LAB_INTEGRATION_SRC_DIR}"
)
