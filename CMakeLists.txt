cmake_minimum_required(VERSION 3.28)
project(Dragon)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(BUILD_TESTS "Build tests cases" ON)

include(FetchContent)

add_definitions(-DSERVER_NAME="Dragon")

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -Wpedantic")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g --coverage -fprofile-arcs -ftest-coverage")

set(CMAKE_CXX_FLAGS_RELEASE
    "-Ofast -flto -march=native -mtune=native -g0 -DNDEBUG")
set(CMAKE_C_FLAGS_RELEASE
    "-Ofast -flto -march=native -mtune=native -g0 -DNDEBUG")

find_package(
  Boost 1.88.0
  COMPONENTS json coroutine program_options system
  REQUIRED)

# Find objects
file(GLOB_RECURSE DRAGON_SOURCES
     "${CMAKE_CURRENT_SOURCE_DIR}/src/objects/dragon/*.cpp")
file(GLOB_RECURSE APP_SOURCES
     "${CMAKE_CURRENT_SOURCE_DIR}/src/objects/app/*.cpp")

# Include headers
include_directories(src/headers)

# Add libraries
add_library(app_core ${APP_SOURCES})
add_library(dragon_core ${DRAGON_SOURCES})

# Add headers
target_include_directories(dragon_core PUBLIC ${Boost_INCLUDE_DIRS}
                                              src/headers/dragon)
target_link_libraries(dragon_core PUBLIC ${Boost_LIBRARIES})
target_include_directories(app_core PUBLIC ${Boost_INCLUDE_DIRS}
                                           src/headers/app)
target_link_libraries(app_core PUBLIC ${Boost_LIBRARIES})

add_executable(dragon src/main.cpp)
target_link_libraries(dragon dragon_core app_core)

if(BUILD_TESTS)
  FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
  )
  FetchContent_MakeAvailable(googletest)

  enable_testing()

  file(GLOB_RECURSE TESTS CONFIGURE_DEPENDS
       "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cc")

  add_executable(tests ${TESTS})

  target_link_libraries(tests GTest::gtest_main dragon_core app_core)
  include(GoogleTest)
  gtest_discover_tests(tests)
endif()
