# Exercise #2 - - Generation of basic objects in the raster

## Instructions for compilation
- The instructions work on computers in CVT. WARNING: The solution on your own PC may not be executable according to the instructions.
- To build the application, the CMake 3.24+ tool and VisualStudio 2019+ or GCC 7.0.0+ (available in CVT) are required.

## Linux
1. Unpack the exercise archive: `unzip izg_lab02.zip`
2. Change the working directory to the exercise folder: `cd izg_lab02/`
3. Create a folder for building the application: `mkdir build`
4. Change the working directory to build: `cd build/`
5. Create the necessary files for building: `cmake ..`
6. Compile the application: `make -j`
7. Run the application: `./izg_lab_02`

## Windows
1. Unpack the program skeleton archive into the Downloads folder (necessary for working in CVT).
2. Open the `cmake-gui` application.
   1. If the computer has CMake version 3.24+, skip this step!
   2. On some computers in CVT, CMake 3.15+ is installed. In this case, download the latest version.
   3. Unpack the archive. The `cmake-gui.exe` file is in the bin/ folder.
   4. Run the `cmake-gui.exe` file and continue with the procedure.
3. Use the "Browse source" button to select the folder with the source code (the folder where the exercise ZIP archive was unpacked).
4. Use the "Browse build" button to enter the path to the source code with the suffix `build/` (for example, `C:/Downloads/izg_lab02/build/`).
5. Click the Generate button and select the VisualStudio 17 (2022 version) option.
6. The `izg_lab_02.sln` file will be located in the `build/` folder.
7. Open the project in VisualStudio 2022 through the `izg_lab_02.sln` file.
8. Use the F5 key to compile and run the application.
