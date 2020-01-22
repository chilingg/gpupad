cmake_minimum_required(VERSION 3.1)
#SET(REDOPERA_SRC )

aux_source_directory(. REDOPERA_SRC)
aux_source_directory(tool/ REDOPERA_SRC)
aux_source_directory(resource/ REDOPERA_SRC)
aux_source_directory(dependent/ REDOPERA_SRC)

option(BUILD_SHARED_LIBS "Build as shared library" ON)

if(BUILD_SHARED_LIBS STREQUAL "ON")
    add_library(redopera STATIC ${REDOPERA_SRC})    # 静态库
else()
    add_library(redopera SHARED ${REDOPERA_SRC})    # 动态库
endif()

include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${CMAKE_BINARY_DIR}/libs)

if (NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Release")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DR_DEBUG)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    link_directories(/usr/local/lib)

    target_link_libraries(redopera librtaudio.so)
    target_link_libraries(redopera pthread) #dl
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_link_libraries(redopera librtaudio.dll.a)
endif()
