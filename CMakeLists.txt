cmake_minimum_required(VERSION 3.3)
project(sample_game)
set(CMAKE_CXX_STANDARD 17)
set(SOURCE_FILES spinning_cube.cpp)
add_executable(sample_game ${SOURCE_FILES})

add_subdirectory(engine)

include_directories("/usr/include")

find_package(glfw3 3.3 REQUIRED)
target_link_libraries(sample_game 14erEngine stbimage glfw libGLEW.so libOpenGL.so)

