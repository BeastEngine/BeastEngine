add_library(
  BeastCommon
  INTERFACE
  "Helpers.hpp"
  "Types.hpp"
  "Filesystem/Types.hpp"
  "UUIDGenerator.hpp"
  "Utils/Hasher.hpp"
  "Utils/Xxh64.hpp"
  "Id.hpp"
)

target_include_directories(BeastCommon INTERFACE "${BeastEngine_SOURCE_DIR}/src")

find_package(xxHash REQUIRED)
find_package(stduuid REQUIRED)
target_link_libraries(BeastCommon INTERFACE xxHash::xxhash stduuid::stduuid)

be_set_compiler_interface_options(BeastCommon)

add_library(BE::common ALIAS BeastCommon)
