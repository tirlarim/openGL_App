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
include_directories(./include)
include_directories(./deps)

file(GLOB_RECURSE RES_SOURCES_SHADERS "shaders/*.*")
file(GLOB_RECURSE RES_SOURCES_TEXTURES "textures/*.*")
set(SRC
    src/main.cpp deps/glad/glad.c src/utils/utils.c
    src/shader.cpp src/shader.hpp
    src/utils/inputWorker.cpp
    src/utils/inputWorker.hpp
    src/utils/graphicUtils.c
    src/utils/graphicUtils.cpp
)

if(APPLE)
  add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${SRC} ${RES_SOURCES_SHADERS} ${RES_SOURCES_TEXTURES})
else()
  add_executable(${PROJECT_NAME} ${SRC})
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "${PROJECT_NAME}"
    MACOSX_BUNDLE TRUE
    MACOSX_FRAMEWORK_IDENTIFIER com.tirlarim.${PROJECT_NAME}
    MACOSX_BUNDLE_GUI_IDENTIFIER com.tirlarim.${PROJECT_NAME}
    MACOSX_BUNDLE_BUNDLE_NAME "${PROJECT_NAME}"
    MACOSX_BUNDLE_BUNDLE_VERSION "0.6.1"
    MACOSX_BUNDLE_SHORT_VERSION_STRING "0.6"
    MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/resources/Info.plist
    RESOURCE ./resources/icon.icns
    XCODE_ATTRIBUTE_ENABLE_HARDENED_RUNTIME TRUE
    XCODE_ATTRIBUTE_EXECUTABLE_NAME "${PROJECT_NAME}"
)

set_source_files_properties(resources/icon.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
target_sources(${PROJECT_NAME} PRIVATE resources/icon.icns)
if(APPLE)
  foreach(RES_FILE ${RES_SOURCES_SHADERS})
    file(RELATIVE_PATH RES_PATH "${CMAKE_CURRENT_SOURCE_DIR}/shaders" ${RES_FILE})
    set_property(SOURCE ${RES_FILE} PROPERTY MACOSX_PACKAGE_LOCATION "Resources/shaders")
  endforeach(RES_FILE)
  foreach(RES_FILE ${RES_SOURCES_TEXTURES})
    file(RELATIVE_PATH RES_PATH "${CMAKE_CURRENT_SOURCE_DIR}/textures" ${RES_FILE})
    set_property(SOURCE ${RES_FILE} PROPERTY MACOSX_PACKAGE_LOCATION "Resources/textures")
  endforeach(RES_FILE)
endif()

target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} ${GLM_LIBRARIES})
