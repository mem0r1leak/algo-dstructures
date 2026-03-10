cmake_minimum_required(VERSION 3.14)
project(algo_dstructures)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE  # Додай цей рядок, щоб прибрати варнінг
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(algo_dstructures main.cpp implementation.h)
add_executable(binary_tree_tests
        tests/binary_tree_tests.cpp
        implementation.h
)

target_link_libraries(binary_tree_tests
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(binary_tree_tests)
