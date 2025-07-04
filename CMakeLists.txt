cmake_minimum_required(VERSION 3.28)
project(Dragon)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(BUILD_TESTS "Build tests cases" ON)

include(FetchContent)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g --coverage -fprofile-arcs -ftest-coverage")

set(CMAKE_CXX_FLAGS_RELEASE "-Ofast -flto -march=native -mtune=native -g0 -DNDEBUG")
set(CMAKE_C_FLAGS_RELEASE "-Ofast -flto -march=native -mtune=native -g0 -DNDEBUG")

find_package(Boost 1.88.0 COMPONENTS
    json coroutine program_options system
REQUIRED)


file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/objects/*.cpp")

include_directories(src/include)

add_library(dragon_core ${SOURCES})

add_executable(dragon src/main.cpp)
target_link_libraries(dragon dragon_core ${Boost_LIBRARIES})


if (BUILD_TESTS)
    fetchcontent_declare(
            googletest
            URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
    )
    fetchcontent_makeavailable(googletest)

    enable_testing()

    file(GLOB_RECURSE TESTS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cc")

    add_executable(tests ${TESTS})

    target_link_libraries(tests GTest::gtest_main dragon_core)
    include(GoogleTest)
    gtest_discover_tests(tests)
endif ()