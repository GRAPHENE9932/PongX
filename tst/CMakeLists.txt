

######################
# Unit tests
######################
#Enable GoogleTest
include(GoogleTest)
enable_testing()
#Find test source files
file(GLOB_RECURSE pongx_tests_SRC ${PROJECT_SOURCE_DIR}/tst/*.cpp)
add_executable(pongx_tests "${pongx_tests_SRC}")
target_link_libraries(pongx_tests PUBLIC pongx_lib gtest gtest_main)
add_test(pongx_tests pongx_tests)
