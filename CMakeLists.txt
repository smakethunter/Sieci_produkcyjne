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

        src/helpers.cpp
        src/factory.cpp
        src/simulation.cpp
        src/reports.cpp
        )

#add_executable(network__debug ${SOURCE_FILES} ../network_exec.cpp)
add_executable(netsim__nodes__debug ${SOURCE_FILES} ../network_exec.cpp)
# == Unit testing using Google Testing Framework ==

set(SOURCES_FILES_TESTS


        #test/test_random.cpp
        #test/test_network.cpp

        #test/test_package.cpp
        #test/test_storage_types.cpp
        #test/test_nodes.cpp
        #test/test_Factory.cpp
        #test/test_simulate.cpp
        test/test_factory_io.cpp)


add_executable(network__test ${SOURCE_FILES} ${SOURCES_FILES_TESTS} test/main_gtest.cpp)
#add_executable(netsim__nodes__test ${SOURCE_FILES} ${SOURCES_FILES_TESTS} test/main_gtest.cpp)
target_include_directories(network__test PUBLIC
        ../../googletest-master/googlemock/include
        ../../googletest-master/googletest/include
        )

add_subdirectory(googletest-master)
target_compile_definitions(network__test PUBLIC EXERCISE_ID=EXERCISE_ID_NODES)
target_link_libraries(network__test gmock)

