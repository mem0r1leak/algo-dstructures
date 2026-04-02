cmake_minimum_required(VERSION 3.14)
project(algo_dstructures)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(algo_dstructures main.cpp)

add_executable(unit_tests
        tests/priority_queue_tests.cpp
)

target_link_libraries(unit_tests
        gtest
        gtest_main
        pthread
)

include(GoogleTest)
gtest_discover_tests(unit_tests)