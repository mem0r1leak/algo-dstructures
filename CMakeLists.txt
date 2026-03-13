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

add_executable(algo_dstructures main.cpp hash-table.h
)
add_executable(tests
        hash-table.h
        tests/hash_table_tests.cpp
)

target_link_libraries(tests
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests)
