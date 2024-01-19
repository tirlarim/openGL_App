cmake_minimum_required(VERSION 3.27)
project(openGL_App CXX C)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "-Werror -Wall -Werror=vla")
set(CMAKE_CXX_FLAGS "-march=native")

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_FLAGS "-Werror -Wall -Werror=vla")
set(CMAKE_C_FLAGS "-march=native")

add_subdirectory(./glfw-3.3) # rewrite
include_directories(./include)

add_executable(openGL_App src/main.cpp src/glad.c
        src/utils/utils.c src/utils/graphicUtils.c
        src/shader.cpp src/shader.hpp)
target_link_libraries(openGL_App glfw)
