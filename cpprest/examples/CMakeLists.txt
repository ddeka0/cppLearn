cmake_minimum_required (VERSION 3.5)
set(CMAKE_BUILD_TYPE Debug)

project(TEST)

message(STATUS "Compiling TestFile : ${PROJECT_NAME}")
set (CMAKE_CXX_STANDARD 11)

message(STATUS "Project Directory: ${PROJECT_SOURCE_DIR}")

set (CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

add_executable("example1" example1.cpp)
add_executable("example2" example2.cpp)
add_executable("example3" example3.cpp)
add_executable("example4" example4.cpp)

add_definitions(-DBOOST_ALL_DYN_LINK)

set(BASIC_LIBRARIES "-lboost_system")

set(REST_LIBRARIES "-lcpprest")

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -g -fsanitize=address")

target_link_libraries("example1" ${BASIC_LIBRARIES} ${REST_LIBRARIES})
target_link_libraries("example2" ${BASIC_LIBRARIES} ${REST_LIBRARIES})
target_link_libraries("example3" ${BASIC_LIBRARIES} ${REST_LIBRARIES})
target_link_libraries("example4" ${BASIC_LIBRARIES} ${REST_LIBRARIES})