cmake_minimum_required(VERSION 3.27)
project(openGL_App C CXX)

set(CMAKE_CXX_STANDARD 23)
add_subdirectory(./glfw-3.3)
include_directories(./include)

add_executable(openGL_App main.cpp src/glad.c)
target_link_libraries(openGL_App glfw)
