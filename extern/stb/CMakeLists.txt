add_library(
  stb INTERFACE
  "include/stb/stb_image.h"
  "include/stb/stb_image_write.h"
)
target_include_directories(stb INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")
