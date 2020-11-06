cmake_minimum_required(VERSION 3.15)
project(PUB)

set(CMAKE_CXX_STANDARD 14)

add_compile_options(-Wall -Wextra -Werror -Wpedantic -pedantic-errors)

include_directories(
        include
)

set(SOURCE_FILES
        src/pub.cpp)

add_executable(PUB main.cpp ${SOURCE_FILES})

# Test configuration using Google Testing Framework

set(SOURCES_FILES_TESTS
        tests/pub_test.cpp)

add_executable(PUB__tests ${SOURCE_FILES} ${SOURCES_FILES_TESTS} tests/main_gtest.cpp)

target_include_directories(PUB__tests PUBLIC
        ../googletest-master/googlemock/include
        ../googletest-master/googletest/include)

add_subdirectory(../googletest-master googletest-master)

target_link_libraries(PUB__tests gmock)
