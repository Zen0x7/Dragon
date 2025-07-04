cmake_minimum_required(VERSION 3.31)
project(Dragon)

set(CMAKE_CXX_STANDARD 20)

find_package(
        Boost 1.88.0
        COMPONENTS
        json
        coroutine
        program_options
        system
        REQUIRED
)


file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/objects/*.cpp")

include_directories(src/include)

add_executable(dragon src/main.cpp ${SOURCES})
target_link_libraries(dragon ${Boost_LIBRARIES})