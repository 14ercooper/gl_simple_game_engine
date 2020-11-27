cmake_minimum_required(VERSION 3.13)
project(14erEngine VERSION 1.0.0 DESCRIPTION "Open Source Light Weight Game Engine")

FILE(GLOB_RECURSE CPP_SOURCES *.cpp)
add_library(14erEngine STATIC ${CPP_SOURCES})
