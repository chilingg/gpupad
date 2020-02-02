cmake_minimum_required(VERSION 3.1)

include_directories(${CMAKE_SOURCE_DIR}/include/redopera)
include_directories(${CMAKE_SOURCE_DIR}/include/redopera/dependents)

link_directories(${CMAKE_BINARY_DIR}/src)

add_executable(unitTest unitTest.cpp)
target_link_libraries(unitTest redopera)

add_executable(sound sound.cpp)
target_link_libraries(sound redopera)

add_executable(threads threads.cpp)
target_link_libraries(threads redopera)

add_executable(window window.cpp)
target_link_libraries(window redopera)

add_executable(font font.cpp)
target_link_libraries(font redopera)
