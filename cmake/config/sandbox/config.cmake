set(SANDBOX_INCLUDE_DIR "${BeastEngine_SOURCE_DIR}/apps")
set(SANDBOX_SRC_DIR "${BeastEngine_SOURCE_DIR}/apps")

set(
	SANDBOX_HEADERS_LIST
	
)
set(
	SANDBOX_SRC_LIST
	"${SANDBOX_SRC_DIR}/sandbox.cpp"
)

file(
	WRITE
	"C:/Users/Tomek/Documents/Projects/C++/BeastEngine/cmake/config/sandbox/sandbox_vars.txt"
	"\$\{SANDBOX_INCLUDE_DIR\}=${SANDBOX_INCLUDE_DIR}\n"
	"\$\{SANDBOX_SRC_DIR\}=${SANDBOX_SRC_DIR}"
)
