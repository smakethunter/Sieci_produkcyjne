cmake_minimum_required(VERSION 3.13)
project(Sieci_produkcyjne)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Wextra -Werror -Wpedantic -pedantic-errors)

include_directories(
        include
)

set(SOURCE_FILES
        src/storage_types.cpp
        src/package.cpp

        src/nodes.cpp

        src/network.cpp

        )

add_executable(network__debug ${SOURCE_FILES} ../network_exec.cpp)

# == Unit testing using Google Testing Framework ==

set(SOURCES_FILES_TESTS
        test/test_network.cpp
        test/test_random.cpp)

add_executable(network__test ${SOURCE_FILES} ${SOURCES_FILES_TESTS} test/main_gtest.cpp)

target_include_directories(network__test PUBLIC
        ../../googletest-master/googlemock/include
        ../../googletest-master/googletest/include
        )

add_subdirectory(googletest-master)

target_link_libraries(network__test gmock)
