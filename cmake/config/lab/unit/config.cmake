set(LAB_UNIT_INCLUDE_DIR "${BeastEngine_SOURCE_DIR}/tests")
set(LAB_UNIT_SRC_DIR "${BeastEngine_SOURCE_DIR}/tests")

set(
	LAB_UNIT_HEADERS_LIST
	"${LAB_UNIT_INCLUDE_DIR}/Unit/Core/Events/KeyboardEventsTest.h"
    "${LAB_UNIT_INCLUDE_DIR}/Unit/Core/Events/MouseEventsTest.h"
    "${LAB_UNIT_INCLUDE_DIR}/Unit/Core/BeastEngineTest.h"
    "${LAB_UNIT_INCLUDE_DIR}/Unit/Core/DataStructuresTests.h"
    "${LAB_UNIT_INCLUDE_DIR}/TestSetup.h"
    "${LAB_UNIT_INCLUDE_DIR}/Unit/Core/Loggers/LoggerTest.h"
    "${LAB_UNIT_INCLUDE_DIR}/Unit/Core/Loggers/LoggersFactoriesTests.h"
    "${LAB_UNIT_INCLUDE_DIR}/TestCase.h"
)
set(
	LAB_UNIT_SRC_LIST
	"${LAB_UNIT_SRC_DIR}/lab_unit.cpp"
    "${LAB_UNIT_SRC_DIR}/Unit/Core/Events/KeyboardEventsTest.cpp"
    "${LAB_UNIT_SRC_DIR}/Unit/Core/Events/MouseEventsTest.cpp"
    "${LAB_UNIT_SRC_DIR}/Unit/Core/BeastEngineTest.cpp"
    "${LAB_UNIT_SRC_DIR}/Unit/Core/DataStructuresTests.cpp"
    "${LAB_UNIT_SRC_DIR}/Unit/Core/Loggers/LoggerTest.cpp"
    "${LAB_UNIT_SRC_DIR}/Unit/Core/Loggers/LoggersFactoriesTests.cpp"
)

file(
	WRITE
	"C:/Users/Tomek/Documents/Projects/C++/BeastEngine/cmake/config/lab/lab_vars.txt"
	"\$\{LAB_UNIT_INCLUDE_DIR\}=${LAB_UNIT_INCLUDE_DIR}\n"
	"\$\{LAB_UNIT_SRC_DIR\}=${LAB_UNIT_SRC_DIR}"
)
