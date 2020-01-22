cmake_minimum_required(VERSION 3.1)

include_directories(${CMAKE_SOURCE_DIR}/src/tool)
include_directories(${CMAKE_SOURCE_DIR}/include)    #  include路径

link_directories(${CMAKE_BINARY_DIR}/src)

add_executable(unitTest unitTest.cpp)
target_link_libraries(unitTest redopera)
