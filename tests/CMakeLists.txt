add_executable(${BEAST_TESTS_TARGET_NAME} lab.cpp)

target_link_libraries(${BEAST_TESTS_TARGET_NAME} PRIVATE ${BEAST_LIB_TARGET_NAME})
target_link_libraries(${BEAST_TESTS_TARGET_NAME} PRIVATE CONAN_PKG::gtest)