# Extracts source files from "dir", returns them in "srcs""
macro(FindSources dir srcs)

    file(GLOB tmp ${dir}/*.cpp)

    #return found files
    set(${srcs} ${tmp})

endmacro()

# Extracts header files from "dir", returns them in "headers""
macro(FindHeaders dir headers)

    file(GLOB tmp ${dir}/*.h)
    
    #return found files
    set(${headers} ${tmp})

endmacro()

macro(getSubdirectories result curdir)

    FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)

    SET(dirlist "")

    FOREACH(child ${children})
        IF(IS_DIRECTORY ${curdir}/${child})
            LIST(APPEND dirlist ${child})
        ENDIF()
    ENDFOREACH()

    SET(${result} ${dirlist})

endmacro()

#
macro(findOsgLib)

    #predefined OSG root
    set(OSG_ROOT  "${CMAKE_CURRENT_SOURCE_DIR}/3rdPartyLibs/OSG" CACHE PATH "OSG root folder")
    set(OSG_DIR  "${CMAKE_CURRENT_SOURCE_DIR}/3rdPartyLibs/OSG" CACHE PATH "OSG root folder")

    FIND_PACKAGE(OpenSceneGraph REQUIRED osgDB osgUtil osgGA osgManipulator osgUtil osgViewer)

    set(OSG_DLLS_DEBUG "" CACHE STRING PRIVATE)
    set(OSG_DLLS_OPTIMIZED "" CACHE STRING PRIVATE)
    set(OSG_PLUGINS_FOLDER "" CACHE STRING PRIVATE)
    set(OSG_PLUGINS_PATH "" CACHE STRING PRIVATE)

    set(OSG_LIBRARIES_TO_INSTALL
      	osg
      	osgDB
      	osgGA
      	osgManipulator
      	osgUtil
      	osgViewer
      	osgText
        OpenThreads
        zlib)
  
    foreach(lib ${OSG_LIBRARIES_TO_INSTALL})
	         LIST(APPEND OSG_DLLS_OPTIMIZED "${CMAKE_CURRENT_SOURCE_DIR}/3rdPartyLibs/OSG/bin/${lib}.dll")
           LIST(APPEND OSG_DLLS_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/3rdPartyLibs/OSG/bin/${lib}d.dll")
    endforeach()

    #file(GLOB OSG_DLLS_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/3rdPartyLibs/OSG/bin/*d.dll)
    #file(GLOB OSG_DLLS_OPTIMIZED ${CMAKE_CURRENT_SOURCE_DIR}/3rdPartyLibs/OSG/bin/*.dll)

    #find plugins subfolder
    getSubdirectories(OSG_PLUGINS_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/3rdPartyLibs/OSG/bin")
    #set full path to plugins subfolder
    set(OSG_PLUGINS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/3rdPartyLibs/OSG/bin/${OSG_PLUGINS_FOLDER}")

endmacro()

macro(CreateTestsTarget)

    set(GTEST_ROOT  "${CMAKE_CURRENT_SOURCE_DIR}/3rdPartyLibs/GTest" CACHE PATH "GTest root folder")

    enable_testing()
    find_package(GTest REQUIRED)
    include(GoogleTest)
                 
    add_executable(studentTests tests/CustomListener.h tests/tests.cpp tests/StudentTests.cpp tests/../src/Student.cpp tests/../include/Student.h)
    target_include_directories(studentTests
                               PRIVATE
                               "${CMAKE_CURRENT_SOURCE_DIR}"
                               "${CMAKE_CURRENT_SOURCE_DIR}/include"
                               "${CMAKE_CURRENT_SOURCE_DIR}/src"
                               "${CMAKE_CURRENT_SOURCE_DIR}/../src"
                               "${CMAKE_CURRENT_SOURCE_DIR}/../include")

    TARGET_LINK_LIBRARIES(studentTests ${OPENSCENEGRAPH_LIBRARIES} )
                  
    target_link_libraries(studentTests GTest::GTest)
    #gtest_discover_tests(studentTests)

    #copy debug dlls
    foreach(file ${OSG_DLLS_DEBUG})
	    add_custom_command(TARGET studentTests POST_BUILD        
    		    		   COMMAND if $<CONFIG:Debug> neq 0 ${CMAKE_COMMAND} -E copy_if_different ${file} $<TARGET_FILE_DIR:studentTests>)      
    endforeach()   

    #copy release dlls
    foreach(file ${OSG_DLLS_OPTIMIZED})
	    add_custom_command(TARGET studentTests POST_BUILD        
    	    			   COMMAND if $<CONFIG:Release> neq 0 ${CMAKE_COMMAND} -E copy_if_different ${file} $<TARGET_FILE_DIR:studentTests>)
    endforeach() 

    #For some reason: https://stackoverflow.com/questions/14714877/mismatch-detected-for-runtimelibrary
    #                 https://stackoverflow.com/questions/14172856/compile-with-mt-instead-of-md-using-cmake
    set_property(TARGET studentTests PROPERTY
                 MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    #set_property(TARGET studentTests PROPERTY
    #             MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")
endmacro()

macro(create_install_post_build_step target_name)

    #copy debug dlls
    foreach(file ${OSG_DLLS_DEBUG})
	    add_custom_command(TARGET ${target_name} POST_BUILD        
    		    		   COMMAND if $<CONFIG:Debug> neq 0 ${CMAKE_COMMAND} -E copy_if_different ${file} $<TARGET_FILE_DIR:${target_name}>)      
    endforeach()   
    
    #copy release dlls
    foreach(file ${OSG_DLLS_OPTIMIZED})
	    add_custom_command(TARGET ${target_name} POST_BUILD        
    	    			   COMMAND if $<CONFIG:Release> neq 0 ${CMAKE_COMMAND} -E copy_if_different ${file} $<TARGET_FILE_DIR:${target_name}>)
    endforeach() 

    #copy osg plugins
    add_custom_command(TARGET ${target_name} POST_BUILD
        			   COMMAND ${CMAKE_COMMAND} -E copy_directory
    	    		   ${OSG_PLUGINS_PATH}
        	    	   $<TARGET_FILE_DIR:${target_name}>/${OSG_PLUGINS_FOLDER})  
              
    #copy bunny
    add_custom_command(TARGET ${target_name} POST_BUILD        
            		   COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/bunny.obj" ${CMAKE_CURRENT_BINARY_DIR})   

endmacro()

#Creates the main executable
macro (CreateMainTarget target_name)

    #c++ 17 standard
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)

    #Get headers and sources
    set(srcs "" CACHE STRING PRIVATE)
    set(headers "" CACHE STRING PRIVATE)

    FindSources("${CMAKE_CURRENT_SOURCE_DIR}/src" srcs)
    FindHeaders("${CMAKE_CURRENT_SOURCE_DIR}/include" headers)

    ADD_EXECUTABLE(${target_name})

    target_sources(${target_name} PRIVATE
                   ${srcs}
                   ${headers})

    INCLUDE_DIRECTORIES(${OPENSCENEGRAPH_INCLUDE_DIRS})
    TARGET_LINK_LIBRARIES(${target_name} ${OPENSCENEGRAPH_LIBRARIES} )

    target_include_directories(${target_name}
                               PRIVATE
                               "${CMAKE_CURRENT_SOURCE_DIR}/include")

    create_install_post_build_step(${target_name})

    set_property(DIRECTORY "${CMAKE_SOURCE_DIR}" PROPERTY VS_STARTUP_PROJECT ${target_name})

endmacro()

