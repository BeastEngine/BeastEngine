#include(FetchContent)

#FetchContent_Declare(
#	box2d
#	GIT_REPOSITORY https://github.com/erincatto/box2d.git
#	GIT_TAG v3.0.0
#	GIT_SHALLOW TRUE
#	GIT_PROGRESS TRUE
#)
#FetchContent_MakeAvailable(box2d)

add_subdirectory(box2d)
