cmake_minimum_required(VERSION 3.19)
project(equirect)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
set(EIGEN_DIR "/home/ben/Documents/cpp/equirect/src/eigen-3.3.9")

# get google tests
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(SOURCE_FILES main.cpp)
add_executable(main ${SOURCE_FILES})

include_directories(src)
include_directories(src/includes)
include_directories(${EIGEN_DIR})

add_subdirectory(src)
add_subdirectory(tests)

target_link_libraries(main equirect pthread X11)

