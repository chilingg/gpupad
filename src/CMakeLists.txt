cmake_minimum_required(VERSION 3.1)
#SET(REDOPERA_SRC )

aux_source_directory(tool/ REDOPERA_SRC)
aux_source_directory(. REDOPERA_SRC)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)    # iinclude路径

option(BUILD_SHARED_LIBS "Build as shared library" ON)

if(BUILD_SHARED_LIBS STREQUAL "ON")
    ADD_LIBRARY(redopera STATIC ${REDOPERA_SRC})    # 静态库
else()
    ADD_LIBRARY(redopera SHARED ${REDOPERA_SRC})    # 动态库
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DR_DEBUG)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    TARGET_LINK_LIBRARIES(redopera pthread) #dl
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    message("In Windows")
endif()

#LINK_DIRECTORIES(directory1 directory2 ...)    # 连接路径
#TARGET_LINK_LIBRARIES(redopera hello)          # 连接路径
