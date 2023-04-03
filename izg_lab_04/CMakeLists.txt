CMAKE_MINIMUM_REQUIRED(VERSION 3.15)
PROJECT( izg04 )

include(${CMAKE_SOURCE_DIR}/CMake/CMakeMarcos.cmake)

findOsgLib()

CreateMainTarget("izg04")

CreateTestsTarget()  
