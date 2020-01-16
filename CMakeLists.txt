cmake_minimum_required(VERSION 3.1)

project(Redopera LANGUAGES CXX)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/dependent)

ADD_SUBDIRECTORY(test test)
ADD_SUBDIRECTORY(src src)
