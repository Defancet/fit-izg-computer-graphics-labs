#
# IZG - Zaklady pocitacove grafiky - FIT VUT
# Lab 01 - Redukce barevneho prostoru
#
# Copyright (c) 2022-23 by Michal Vlnas, ivlnas@fit.vutbr.cz
#
# Tato sablona slouzi vyhradne pro studijni ucely, sireni kodu je bez vedomi autora zakazano.
#

CMAKE_MINIMUM_REQUIRED(VERSION 3.12)
PROJECT(izg_lab_01 LANGUAGES C CXX)

SET(CMAKE_CXX_STANDARD 17)

SET(SOURCES
    base.cpp
    student.cpp
    main.cpp
)

SET(HEADERS
    base.h
)

OPTION(SDL_SHARED "" OFF)
OPTION(SDL_STATIC "" ON)
ADD_SUBDIRECTORY(SDL)
ADD_LIBRARY(SDL2::SDL2 ALIAS SDL2-static)
ADD_LIBRARY(SDL2::SDL2Main ALIAS SDL2main)

ADD_EXECUTABLE(${PROJECT_NAME} ${HEADERS} ${SOURCES} main.cpp)

if (MSVC)
    TARGET_COMPILE_OPTIONS(${PROJECT_NAME} PRIVATE /W3)
ELSE()
    TARGET_COMPILE_OPTIONS(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
ENDIF()

SET_PROPERTY(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
    SDL2::SDL2
    SDL2::SDL2Main
)

