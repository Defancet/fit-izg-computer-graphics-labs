#
# IZG - Zaklady pocitacove grafiky - FIT VUT
# Lab 05 - Zobrazování 2D křivek v rastru
#
# Copyright (c) 2022 by Michal Vlnas, ivlnas@fit.vutbr.cz
#               Modified by Tomáš Polášek, ipolasek@fit.vutbr.cz
#		Modified by Bořek Reich, ireich@fit.vutbr.cz
#
# Tato sablona slouzi vyhradne pro studijni ucely, sireni kodu je bez vedomi autora zakazano.
#

cmake_minimum_required (VERSION 3.12)
project (izg_lab_05 LANGUAGES C CXX)

set (CMAKE_CXX_STANDARD 17)

set (SOURCE_FILES
    main.cpp
    color.cpp
    student.cpp
    vector.cpp
)

set (HEADER_FILES
    base.h
    color.h
    student.h
    globals.h
    vector.h
    main.h
)

# Output built files to ./build/
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../build )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../build )

# Setup SDL library
option (SDL_SHARED "" OFF)
option (SDL_STATIC "" ON)
add_subdirectory (SDL)
add_library (SDL2::SDL2 ALIAS SDL2-static)
add_library (SDL2::SDL2Main ALIAS SDL2main)

# Add the main exercise application
add_executable (${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

# Link with libraries
target_link_libraries (${PROJECT_NAME}
    SDL2::SDL2
    SDL2::SDL2Main
)

# Supply compilation flags based on the platform
if (MSVC)
    target_compile_options (${PROJECT_NAME} PRIVATE /W3)
else ()
    target_compile_options (${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
endif ()

# Support for Visual Studio
set_property (DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
