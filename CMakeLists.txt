cmake_minimum_required(VERSION 3.27)
project(openGL_App CXX C)

set(ERROR_FLAGS "-Werror -Wall -Werror=vla")
set(OPTIMIZATION_FLAGS "-march=native")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS ${ERROR_FLAGS})
set(CMAKE_CXX_FLAGS ${OPTIMIZATION_FLAGS})

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_FLAGS ${ERROR_FLAGS})
set(CMAKE_C_FLAGS ${OPTIMIZATION_FLAGS})

add_subdirectory(./glfw-3.3)

find_package(glm REQUIRED)
if (glm_FOUND)
    include_directories(${GLM_INCLUDE_DIRS})
else ()
    message(FATAL_ERROR "Error: Unable to find GLM. Try try install it:")
    if (APPLE) # brew one love
        message(STATUS "$ brew install glm") # For macOS
    elseif (WIN32) # Install GLM via vcpkg
        message(STATUS ".\\vcpkg install glm") # For Windows
        message(STATUS "This command will download and install GLM for you, making it available for your projects.")
        message(STATUS "For Visual Studio projects, vcpkg integrates automatically after the installation,
        or you may need to run `$ .\\vcpkg integrate install` in your vcpkg directory.")
    elseif (UNIX AND NOT APPLE)
        # Determine the Linux distribution and suggest an installation command
        execute_process(COMMAND cat /etc/os-release OUTPUT_VARIABLE OS_RELEASE_CONTENTS)
        if (OS_RELEASE_CONTENTS MATCHES "ID_LIKE=debian")
            set(INSTALL_COMMAND "sudo apt-get install libglm-dev")
        elseif (OS_RELEASE_CONTENTS MATCHES "ID=fedora" OR OS_RELEASE_CONTENTS MATCHES "ID_LIKE=fedora")
            set(INSTALL_COMMAND "sudo dnf install glm-devel")
        elseif (OS_RELEASE_CONTENTS MATCHES "ID=arch")
            set(INSTALL_COMMAND "sudo pacman -S glm")
        else ()
            set(INSTALL_COMMAND "Please install GLM for your Linux distribution")
        endif ()
          message(FATAL_ERROR "GLM not found. To install GLM, run: ${INSTALL_COMMAND}")
    endif ()
endif ()

include_directories(./include)

add_executable(${PROJECT_NAME} src/main.cpp src/glad.c
        src/utils/utils.c src/utils/graphicUtils.c
        src/shader.cpp)
target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} ${GLM_LIBRARIES})
