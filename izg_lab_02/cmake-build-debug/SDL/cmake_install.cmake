# Install script for directory: C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/izg_lab_02")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "C:/Program Files/JetBrains/CLion 2022.3.2/bin/mingw/bin/objdump.exe")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/cmake-build-debug/SDL/libSDL2maind.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/cmake-build-debug/SDL/libSDL2d.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL2/SDL2mainTargets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL2/SDL2mainTargets.cmake"
         "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/cmake-build-debug/SDL/CMakeFiles/Export/f084604df1a27ef5b4fef7c7544737d1/SDL2mainTargets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL2/SDL2mainTargets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL2/SDL2mainTargets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL2" TYPE FILE FILES "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/cmake-build-debug/SDL/CMakeFiles/Export/f084604df1a27ef5b4fef7c7544737d1/SDL2mainTargets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL2" TYPE FILE FILES "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/cmake-build-debug/SDL/CMakeFiles/Export/f084604df1a27ef5b4fef7c7544737d1/SDL2mainTargets-debug.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL2/SDL2staticTargets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL2/SDL2staticTargets.cmake"
         "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/cmake-build-debug/SDL/CMakeFiles/Export/f084604df1a27ef5b4fef7c7544737d1/SDL2staticTargets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL2/SDL2staticTargets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL2/SDL2staticTargets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL2" TYPE FILE FILES "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/cmake-build-debug/SDL/CMakeFiles/Export/f084604df1a27ef5b4fef7c7544737d1/SDL2staticTargets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL2" TYPE FILE FILES "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/cmake-build-debug/SDL/CMakeFiles/Export/f084604df1a27ef5b4fef7c7544737d1/SDL2staticTargets-debug.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Devel" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL2" TYPE FILE FILES
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/SDL2Config.cmake"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/cmake-build-debug/SDL2ConfigVersion.cmake"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/SDL2" TYPE FILE FILES
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_assert.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_atomic.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_audio.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_bits.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_blendmode.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_clipboard.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_config_android.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_config_emscripten.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_config_iphoneos.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_config_macosx.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_config_minimal.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_config_os2.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_config_pandora.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_config_windows.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_config_winrt.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_copying.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_cpuinfo.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_egl.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_endian.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_error.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_events.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_filesystem.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_gamecontroller.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_gesture.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_haptic.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_hidapi.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_hints.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_joystick.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_keyboard.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_keycode.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_loadso.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_locale.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_log.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_main.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_messagebox.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_metal.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_misc.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_mouse.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_mutex.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_name.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_opengl.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_opengl_glext.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_opengles.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_opengles2.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_opengles2_gl2.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_opengles2_gl2ext.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_opengles2_gl2platform.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_opengles2_khrplatform.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_pixels.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_platform.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_power.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_quit.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_rect.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_render.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_rwops.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_scancode.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_sensor.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_shape.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_stdinc.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_surface.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_system.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_syswm.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_test.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_test_assert.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_test_common.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_test_compare.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_test_crc32.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_test_font.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_test_fuzzer.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_test_harness.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_test_images.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_test_log.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_test_md5.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_test_memory.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_test_random.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_thread.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_timer.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_touch.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_types.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_version.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_video.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/SDL_vulkan.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/begin_code.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/include/close_code.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/cmake-build-debug/SDL/include/SDL_config.h"
    "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/cmake-build-debug/SDL/include/SDL_revision.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/cmake-build-debug/SDL/sdl2.pc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE PROGRAM FILES "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/cmake-build-debug/SDL/sdl2-config")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/aclocal" TYPE FILE FILES "C:/Users/gmamy/Desktop/bit1/leto/IZG/izg_lab_02/SDL/sdl2.m4")
endif()

