project(Google_tests)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(Google_Tests_run test_sphere.cpp)
target_link_libraries(Google_Tests_run gtest gtest_main equirect)

enable_testing()
include(GoogleTest)
gtest_discover_tests(Google_Tests_run)