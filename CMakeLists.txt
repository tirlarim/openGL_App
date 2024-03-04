cmake_minimum_required(VERSION 3.27)
project(openGL_App CXX C)

set(DEV_DOMAIN com)
set(DEV_NAME tirlarim)
set(MACOS_INFO_PLIST_PATH ${CMAKE_CURRENT_SOURCE_DIR}/resources/Info.plist)

#set(ERROR_FLAGS "-Werror -Wall")
set(CUSTOM_ERROR_FLAGS "-Werror=vla")
set(OPTIMIZATION_FLAGS "-march=native")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_FLAGS ${ERROR_FLAGS}\ ${OPTIMIZATION_FLAGS})

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_FLAGS ${ERROR_FLAGS}\ ${OPTIMIZATION_FLAGS})

add_subdirectory(./deps/glfw-3.3)
add_subdirectory(./deps/assimp)
include_directories(./include)
include_directories(./deps)

set(SRC
    src/main.cpp deps/glad/glad.c src/utils/utils.c src/utils/graphicUtils.c
    src/Shader.cpp src/Shader.hpp
    src/inputWorker.cpp src/inputWorker.hpp
    src/Camera.cpp src/Camera.hpp
    src/Mesh.cpp src/Mesh.hpp
    src/Model.cpp src/Model.hpp
)

if(APPLE)
  add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${SRC})
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
      MACOSX_BUNDLE_INFO_PLIST ${MACOS_INFO_PLIST_PATH}
#      RESOURCE ${MACOS_ICON_PATH}
      XCODE_ATTRIBUTE_ENABLE_HARDENED_RUNTIME TRUE
      XCODE_ATTRIBUTE_EXECUTABLE_NAME ${PROJECT_NAME}
  )
  add_custom_target(copy_resources_files
      COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${PROJECT_NAME}>/../Resources
      COMMAND rsync -rv --checksum
      --exclude='Info.plist'
      ${CMAKE_CURRENT_SOURCE_DIR}/resources/ $<TARGET_FILE_DIR:${PROJECT_NAME}>/../Resources
      COMMAND rsync -rv --checksum ${CMAKE_CURRENT_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:${PROJECT_NAME}>/../Resources
  )
endif()

target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} assimp)
target_link_libraries(${PROJECT_NAME} ${GLM_LIBRARIES})
add_dependencies(${PROJECT_NAME} copy_resources_files)

#add_custom_command(TARGET ${PROJECT_NAME}
#    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${PROJECT_NAME}>/../Resources
#    COMMAND rsync -rv --checksum
#              --exclude='Info.plist'
#              ${CMAKE_CURRENT_SOURCE_DIR}/resources/ $<TARGET_FILE_DIR:${PROJECT_NAME}>/../Resources
#    COMMAND rsync -rv ${CMAKE_CURRENT_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:${PROJECT_NAME}>/../Resources
#    #    FOR DEBUG ONLY
#    #    COMMAND pwd
#    #    COMMAND echo ${CMAKE_CURRENT_SOURCE_DIR}
#    #    COMMAND ls -lsa ${CMAKE_CURRENT_SOURCE_DIR}/resources
#    #    COMMAND echo $<TARGET_FILE_DIR:${PROJECT_NAME}>/../Resources
#    #    COMMAND ls -lsa $<TARGET_FILE_DIR:${PROJECT_NAME}>/../Resources
#)
