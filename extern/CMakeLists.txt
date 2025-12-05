#include(FetchContent)

#FetchContent_Declare(
#	stb
#	GIT_REPOSITORY https://github.com/nothings/stb.git
#	GIT_BRANCH master
#	GIT_SHALLOW TRUE
#	GIT_PROGRESS TRUE
#)
#FetchContent_MakeAvailable(stb)

add_subdirectory(box2d)
add_subdirectory(stb)
