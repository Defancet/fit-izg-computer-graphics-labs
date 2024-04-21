cmake_minimum_required(VERSION 3.12.0)

project(
  ArgumentViewer
  VERSION 1.0.2
  )

#set these variables to *.cpp, *.c, ..., *.h, *.hpp, ...
set(SOURCES 
  src/${PROJECT_NAME}/ArgumentViewer.cpp
  src/${PROJECT_NAME}/private/ArgumentViewerImpl.cpp
  src/${PROJECT_NAME}/private/Format.cpp
  src/${PROJECT_NAME}/private/ValueFormat.cpp
  src/${PROJECT_NAME}/private/CommonFunctions.cpp
  src/${PROJECT_NAME}/private/LineSplitter.cpp
  src/${PROJECT_NAME}/private/IsPresentFormat.cpp
  src/${PROJECT_NAME}/private/Globals.cpp
  src/${PROJECT_NAME}/private/ArgumentListFormat.cpp
  src/${PROJECT_NAME}/private/ContextFormat.cpp
  src/${PROJECT_NAME}/private/StringVectorFormat.cpp
  )
set(PRIVATE_INCLUDES 
  src/${PROJECT_NAME}/private/ArgumentViewerImpl.h
  src/${PROJECT_NAME}/private/Format.h
  src/${PROJECT_NAME}/private/ValueFormat.h
  src/${PROJECT_NAME}/private/SingleValueFormat.h
  src/${PROJECT_NAME}/private/LineSplitter.h
  src/${PROJECT_NAME}/private/VectorFormat.h
  src/${PROJECT_NAME}/private/StringVectorFormat.h
  src/${PROJECT_NAME}/private/IsPresentFormat.h
  src/${PROJECT_NAME}/private/ArgumentListFormat.h
  src/${PROJECT_NAME}/private/ContextFormat.h
  src/${PROJECT_NAME}/private/CommonFunctions.h
  src/${PROJECT_NAME}/private/Globals.h  
  )
set(PUBLIC_INCLUDES
  src/${PROJECT_NAME}/ArgumentViewer.h
  src/${PROJECT_NAME}/Fwd.h
  src/${PROJECT_NAME}/Exception.h
  )
set(INTERFACE_INCLUDES )

#list of libraries to find
#it should contain list of "lists"
#format:
#set(ExternLibraries
#  A\\ B\\ C\\ D
#  E\\ F\\ G
#  )
#It will be converted into:
#find_package(A B C D)
#find_package(E F G)
#If version is specified, it has to be the second parameter (B)
set(ExternPrivateLibraries )
set(ExternPublicLibraries 
  MealyMachine\\ 1.0.0\\ CONFIG
  TxtUtils\\ 1.0.0\\ CONFIG
  )
set(ExternInterfaceLibraries )

#set these variables to targets
set(PrivateTargets )
set(PublicTargets MealyMachine::MealyMachine TxtUtils::TxtUtils)
set(InterfaceTargets )

#set these libraries to variables that are provided by libraries that does not support configs
set(PrivateIncludeVariables )
set(PublicIncludeVariables )
set(InterfaceIncludeVariables )
set(PrivateReleaseLibraryVariables )
set(PublicReleaseLibraryVariables )
set(InterfaceReleaseLibraryVariables )
set(PrivateDebugLibraryVariables )
set(PublicDebugLibraryVariables )
set(InterfaceDebugLibraryVariables )

SET(CMAKE_CXX_STANDARD 14)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

option(${PROJECT_NAME}_BUILD_TESTS "toggle building of unit tests")
if(${PROJECT_NAME}_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
  add_test(NAME baseTest COMMAND tests)
endif()

include(CMakeUtils.cmake)
