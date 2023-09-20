set(
	BEAST_API_LIST
	"Exceptions.h"
	"Helpers.h"
	"Types.h"
   "Filesystem/Types.h" "IdGenerators/IUuIdGenerator.h" "IdGenerators/UuId4Generator.h" "Utils/Hasher.h")

set(
	BE_SRC_LIST
  "IdGenerators/UuId4Generator.cpp")

add_library(BeastCommon STATIC "${BE_SRC_LIST}" "${BEAST_API_LIST}" "Utils/Xxh64.h" "Utils/String.h")
target_include_directories(BeastCommon PUBLIC "${BeastEngine_SOURCE_DIR}/src")
target_compile_features(BeastCommon PUBLIC cxx_std_20)

find_package(xxHash REQUIRED)
find_package(stduuid REQUIRED)
target_link_libraries(
	BeastCommon
    PUBLIC
		xxHash::xxhash
		stduuid::stduuid
)

be_set_compiler_options(BeastCommon)

add_library(BE::common ALIAS BeastCommon)
