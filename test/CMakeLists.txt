cmake_minimum_required(VERSION 3.1)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/tool)
LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/src)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)    # iinclude路径

add_executable(unitTest unitTest.cpp)
TARGET_LINK_LIBRARIES(unitTest redopera)
