cmake_minimum_required(VERSION 3.27)
project(openGL_App C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_FLAGS "-Werror -Wall -Werror=vla")
set(CMAKE_C_FLAGS "-march=native")

add_subdirectory(./glfw-3.3) # rewrite
include_directories(./include)

add_executable(openGL_App src/main.c src/glad.c
        src/utils/utils.c)
target_link_libraries(openGL_App glfw)
