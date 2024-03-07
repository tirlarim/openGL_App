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

set(CPPCHECK_CACHE_DIR ./cppcheckCache)

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

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 23)
set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 23)
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
  # Ignore -Wunused-parameter warning (for glfw callbacks)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-parameter")
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

add_custom_target(check_files
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CPPCHECK_CACHE_DIR}
    COMMAND cppcheck
    --cppcheck-build-dir=${CPPCHECK_CACHE_DIR}
    --check-level=exhaustive
    --enable=warning,performance,portability
    --error-exitcode=0 ${SRC}
)

target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} assimp)
target_link_libraries(${PROJECT_NAME} ${GLM_LIBRARIES})
add_dependencies(${PROJECT_NAME} copy_resources_files)
add_dependencies(${PROJECT_NAME} check_files)
