cmake_minimum_required(VERSION 3.13)
project(14erEngine VERSION 1.0.0 DESCRIPTION "Open Source Light Weight Game Engine")

add_library(14erEngine STATIC GameEngine.cpp ShaderProgram.cpp Object.cpp Model.cpp Collider.cpp Material.cpp Script.cpp Vertex.cpp Camera.cpp math/Quaternion.cpp materials/MatteSkyBlue.cpp models/ColoredCube.cpp)
