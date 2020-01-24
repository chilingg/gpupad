cmake_minimum_required(VERSION 3.1)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include/dependents)

link_directories(${CMAKE_BINARY_DIR}/src)

add_executable(unitTest unitTest.cpp)
target_link_libraries(unitTest redopera)
