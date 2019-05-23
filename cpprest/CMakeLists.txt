cmake_minimum_required (VERSION 3.5)
set(CMAKE_BUILD_TYPE Debug)

project(TEST)

message(STATUS "Compiling TestFile : ${PROJECT_NAME}")
set (CMAKE_CXX_STANDARD 17)

message(STATUS "Project Directory: ${PROJECT_SOURCE_DIR}")

set (CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

add_executable(${PROJECT_NAME} test.cpp)

add_definitions(-DBOOST_ALL_DYN_LINK)

set(BASIC_LIBRARIES "-lboost_system -lcrypto -lssl -lboost_log_setup -lboost_log -lboost_thread \
                      -lpthread")

set(REST_LIBRARIES "-lcpprest -lyaml-cpp")

set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -g -fsanitize=address")

target_link_libraries(${PROJECT_NAME} ${BASIC_LIBRARIES} ${REST_LIBRARIES})
