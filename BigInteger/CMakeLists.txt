cmake_minimum_required(VERSION 3.14)
project(IntegerProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")


include(FetchContent)
include_directories(include)

# Fetch GoogleTest
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Build the main Integer library
add_library(integer_lib
    src/Integer.cpp
)

# Build the executable for manual testing (optional)
add_executable(main src/main.cpp)
target_link_libraries(main integer_lib)

# Unit tests
file(GLOB TEST_SOURCES "tests/*.cpp")

add_executable(integer_tests ${TEST_SOURCES})
target_link_libraries(integer_tests gtest_main integer_lib)
include(GoogleTest)
gtest_discover_tests(integer_tests)
