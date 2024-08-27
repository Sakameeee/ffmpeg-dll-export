cmake_minimum_required(VERSION 3.10)
project(FFMPEG CXX)

set(CMAKE_CXX_STANDARD 11)

option(LINK_SHARED_LIBS "link shared library" OFF)
option(LINK_STATIC_LIBS "link static library" ON)

add_subdirectory(src)

