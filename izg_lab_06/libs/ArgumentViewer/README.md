# ArgumentViewer
Simple cpp library for command line argument parsing.

## Building
The library depends on two other libraries:
[MealyMachine](https://github.com/dormon/MealyMachine)
[TxtUtils](https://github.com/dormon/TxtUtils)
```
$ mkdir build
$ cd build
$ cmake-gui .. # or ccmake ..
$ #set MEALYMACHINE_DIR and TXTUTILS_DIR to paths to MealyMachineConfig.cmake and TxtUtilsConfig.cmake
$ make
```
You have to build and install these libraries first.

## How to use this library
```cpp

#include<memory>
#include<ArgumentViewer/ArgumentViewer.h>

int main(int argc,char*argv[]){
  
  auto args = std::make_shared<ArgumentViewer>(argc,argv);

  auto modelName  = args->gets("--model","2tri.3ds","model file name");

  auto windowSize = arg->getu32v("--window-size",{512,512},"window size" );

  auto lightPosition = arg->getf32v("--light",{0.f,1000.f,0.f,1.f},"light position");

  bool printHelp = arg->isPresent("-h","prints this help");

  printHelp = printHelp || !arg->validate();
  if(printHelp){
    std::cerr<<arg->toStr();
    exit(0);
  }

  //...
}

```

## Features
* Arguments in text file
```
$ ./yourApp --param 1 \< file.txt
```
* Context of arguments
```cpp
//You can specify context of arguments - cathegory of arguments
auto a= make_shared<ArgumentViewer>(argc,argv);
auto skip     = a->isPresent("--skip");
auto run      = a->isPresent("--run");
auto play     = a->isPresent("--play");
auto send_msg = a->isPresent("--send_msg");
light.att = a->getContext("light")->isPresent("att");
light.col = a->getContext("light")->getf32v("color");
light.pos = a->getContext("light")->getf32v("pos");
```
```
$ ./yourApp --skip light \{ att color 1.f 1.f 1.f pos 0.f 0.f 0.f \}
```
* Automatic help / nice help output
```
#example of help output

length = 10   [f32] - x size of vector
width  = 11.1 [f64] - y size of vector
xoff   = 0    [u32] - x offset of window
xpix   = 1000 [i32] - x size of window in pixels
yoff   = 0    [u64] - y offset of window
ypix   = 1331 [i64] - y size of window in pixels
light { - attributes of light source
  color    = 1 0 0  [f64*  ] - normalized color
  name     = light  [string] - name identificator
  position = 0 0 0  [f32*  ] - world space position
  info { - additional informations
    attributes       = {castShadows
                       movable
                       scriptable
                       printable}     [string*] - additional attributes
    bytesPerColor    = 1 2 1          [u32*   ] - number of bytes per color channel
    bytesPerPosition = 4 8 4          [u64*   ] - number of bytes per position position
    directional                                 - changes omnidirectional light into directional
    sampling         = 10 10 0 0 0 0
                       0 0 0 1 128 1
                       3              [i32*   ] - sampling of area light sources
    soffset          = 0 0            [i64*   ] - offset of sampling area
  }
}
```
## Exceptions
ArgumentViewer throws two kinds of exceptions - argumentViewer::ex::Exception and argumentViewer::ex::MatchError.
Exceptions are inherited from std::runtime_error.
ex::MatchError is thrown if you pass wrong arguments to the application.
ex::Exception is thrown if you make an error in creation of ArgumentViewer object.
