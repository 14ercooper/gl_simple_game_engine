cmake_minimum_required(VERSION 3.3)
project(a7)
set(CMAKE_CXX_STANDARD 17)
set(SOURCE_FILES spinning_cube.cpp)

set(CMAKE_CXX_FLAGS "-O3")

add_executable(a7 ${SOURCE_FILES})

add_subdirectory(engine)

find_package(glfw3 3.3 REQUIRED)
target_link_libraries(a7 PUBLIC 14erEngine stbimage glfw libGLEW.so libOpenGL.so)
