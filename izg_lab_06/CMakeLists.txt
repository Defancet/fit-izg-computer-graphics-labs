cmake_minimum_required(VERSION 3.19)

project(izg_lab06)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1 ) 

add_library(glm INTERFACE)
target_include_directories(glm INTERFACE libs/glm-0.9.9.8)

add_subdirectory(libs/MealyMachine)
add_subdirectory(libs/TxtUtils)
add_subdirectory(libs/ArgumentViewer)
add_subdirectory(libs/geGL)
add_subdirectory(libs/BasicCamera)


option(USE_SYSTEM_SDL2 "if this is set, cmake will try to use system SDL2.")

if(USE_SYSTEM_SDL2)
  find_package(SDL2 REQUIRED)
else()
  option(SDL_SHARED "" OFF)
  option(SDL_STATIC "" ON)
  add_subdirectory(libs/SDL-release-2.26.3)
  #add_library(SDL2::SDL2 ALIAS SDL2-static)
  #add_library(SDL2::SDL2main ALIAS SDL2main)
endif()

set(LIBS_SOURCES
  libs/stb_image/stb_image.h
  libs/stb_image/stb_image_write.h
  libs/impl.cpp
  )

source_group("libs"      FILES ${LIBS_SOURCES})

add_executable(${PROJECT_NAME} 
  src/main.cpp
  src/timer.hpp
  ${LIBS_SOURCES}
  )

add_custom_target(run ./${PROJECT_NAME})

if (CMAKE_CROSSCOMPILING)
  target_link_libraries(${PROJECT_NAME}  
    -lmingw32
    -static-libgcc 
    -static-libstdc++
    )
endif()

target_link_libraries(${PROJECT_NAME} 
  SDL2::SDL2
  SDL2::SDL2main
  ArgumentViewer::ArgumentViewer
  geGL::geGL
  BasicCamera::BasicCamera
  )

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/libs/stb_image)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

option(CLEAR_CMAKE_ROOT_DIR "if this is set, #define CMAKE_ROOT_DIR will be .")

if(NOT CLEAR_CMAKE_ROOT_DIR)
  target_compile_definitions(${PROJECT_NAME} PUBLIC CMAKE_ROOT_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
else()
  target_compile_definitions(${PROJECT_NAME} PUBLIC CMAKE_ROOT_DIR=".")
endif()
