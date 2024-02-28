cmake_minimum_required(VERSION 3.27)
project(openGL_App CXX C)

set(DEV_DOMAIN com)
set(DEV_NAME tirlarim)
set(MACOS_ICON_PATH ./resources/icon.icns)

#set(ERROR_FLAGS "-Werror -Wall")
set(CUSTOM_ERROR_FLAGS "-Werror=vla")
set(OPTIMIZATION_FLAGS "-march=native")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_FLAGS ${ERROR_FLAGS}\ ${OPTIMIZATION_FLAGS})

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_FLAGS ${ERROR_FLAGS}\ ${OPTIMIZATION_FLAGS})

add_subdirectory(./glfw-3.3)
include_directories(./include)
include_directories(./deps)

file(GLOB_RECURSE RES_SOURCES_SHADERS "shaders/*.*")
file(GLOB_RECURSE RES_SOURCES_TEXTURES "textures/*.*")
file(GLOB_RECURSE RES_SOURCES_MATERIALS "resources/materials/*.*")

set(RESOURCES
    ${RES_SOURCES_SHADERS}
    ${RES_SOURCES_TEXTURES}
    ${RES_SOURCES_MATERIALS}
)

set(SRC
    src/main.cpp deps/glad/glad.c src/utils/utils.c src/utils/graphicUtils.c
    src/shader.cpp src/shader.hpp
    src/inputWorker.cpp src/inputWorker.hpp
    src/camera.cpp src/camera.hpp
)

if(APPLE)
  add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${SRC} ${RESOURCES})
else()
  add_executable(${PROJECT_NAME} ${SRC})
endif()

if(APPLE)
  set_target_properties(${PROJECT_NAME} PROPERTIES
      OUTPUT_NAME ${PROJECT_NAME}
      MACOSX_BUNDLE TRUE
      MACOSX_FRAMEWORK_IDENTIFIER ${DEV_DOMAIN}.${DEV_NAME}.${PROJECT_NAME}
      MACOSX_BUNDLE_GUI_IDENTIFIER ${DEV_DOMAIN}.${DEV_NAME}.${PROJECT_NAME}
      MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME}
      MACOSX_BUNDLE_BUNDLE_VERSION "0.11.1"
      MACOSX_BUNDLE_SHORT_VERSION_STRING "0.11"
      MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/resources/Info.plist
      RESOURCE ${MACOS_ICON_PATH}
      XCODE_ATTRIBUTE_ENABLE_HARDENED_RUNTIME TRUE
      XCODE_ATTRIBUTE_EXECUTABLE_NAME ${PROJECT_NAME}
  )
  set_source_files_properties(${MACOS_ICON_PATH} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
  target_sources(${PROJECT_NAME} PRIVATE ${MACOS_ICON_PATH})
  foreach(RES_FILE ${RES_SOURCES_SHADERS})
    file(RELATIVE_PATH RES_PATH "${CMAKE_CURRENT_SOURCE_DIR}/shaders" ${RES_FILE})
    set_property(SOURCE ${RES_FILE} PROPERTY MACOSX_PACKAGE_LOCATION "Resources/shaders")
  endforeach(RES_FILE)
  foreach(RES_FILE ${RES_SOURCES_TEXTURES})
    file(RELATIVE_PATH RES_PATH "${CMAKE_CURRENT_SOURCE_DIR}/textures" ${RES_FILE})
    set_property(SOURCE ${RES_FILE} PROPERTY MACOSX_PACKAGE_LOCATION "Resources/textures")
  endforeach(RES_FILE)
  foreach(RES_FILE ${RES_SOURCES_MATERIALS})
    file(RELATIVE_PATH RES_PATH "${CMAKE_CURRENT_SOURCE_DIR}/resources/materials" ${RES_FILE})
    set_property(SOURCE ${RES_FILE} PROPERTY MACOSX_PACKAGE_LOCATION "Resources/materials")
  endforeach(RES_FILE)
endif()

target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} ${GLM_LIBRARIES})
