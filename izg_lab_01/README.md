# Exercise #1 - Color space reduction

## Instructions for compilation
- Visual Studio 2019 or GCC 7.0.0+ and CMake 3.12.0+ are required to compile

## Linux
- mkdir build
- cd build/
- cmake ..
- make -j 4

In case of startup problems, check that you have the required dependencies for rendering with SDL, especially make sure your graphics drivers are up to date. SDL does not need to be installed, it is bundled with the program framework. Complete dependencies for Ubuntu 20.04 can be found here:

https://packages.ubuntu.com/en/source/focal/libsdl2

Most of the packages are already part of the base system, compiler or graphics driver, but some may be missing on some distributions and cause crashes.
## Windows
- Start the CMake GUI
- Press Browse Source and select the folder with the unzipped exercise
- Press Browse Build and select the same address with the extension build/, i.e. e.g. pathToFolder/izg_lab_01/build/
- Click Configure
- Then click on Generate
- In the build/ folder there will now be a file izg_lab_01.sln, run Visual Studio through it
- Next, the application can be assembled directly and run
