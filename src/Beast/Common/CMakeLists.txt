add_library(
	BeastCommon
	INTERFACE
		"Exceptions.h"
		"Helpers.h"
		"Types.h"
		"Filesystem/Types.h"
		"UUIDGenerator.h"
		"Utils/Hasher.h"
		"Utils/Xxh64.h"
		"Utils/String.h"
)

target_include_directories(BeastCommon INTERFACE "${BeastEngine_SOURCE_DIR}/src")

find_package(xxHash REQUIRED)
find_package(stduuid REQUIRED)
target_link_libraries(
	BeastCommon
    INTERFACE
		xxHash::xxhash
		stduuid::stduuid
)

be_set_compiler_interface_options(BeastCommon)

add_library(BE::common ALIAS BeastCommon)
