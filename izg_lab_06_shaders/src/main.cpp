#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <SDL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <BasicCamera/OrbitCamera.h>
#include <BasicCamera/PerspectiveCamera.h>

#include <ArgumentViewer/ArgumentViewer.h>

#include <geGL/geGL.h>
#include <geGL/StaticCalls.h>

#include <libs/stb_image/stb_image_write.h>
#include <libs/stb_image/stb_image.h>

#include <timer.hpp>

using namespace ge::gl;

bool errorCmd=false;

void error(std::string const&name,std::string const&msg){
  if(errorCmd){
    std::cerr << name << std::endl;
    std::cerr << msg << std::endl;
  }else{
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,name.c_str(),msg.c_str(),nullptr);
  }
}

std::string loadTxtFile(std::string const&fileName){
  auto file = std::ifstream(fileName);
  if(!file.is_open()){
    error("cannot open file: "+fileName,"text file: "+fileName+" cannot be opened");
    return "";
  }
  std::string str((std::istreambuf_iterator<char>(file)),
                 std::istreambuf_iterator<char>());
  return str;
}

std::string shaderTypeToName(GLuint type){
  if(type==GL_VERTEX_SHADER  )return "vertex";
  if(type==GL_FRAGMENT_SHADER)return "fragment";
  return "unknown";
}

GLuint createShader(GLuint type,std::string const&src){
  GLuint vs = glCreateShader(type);
  char const*vsSrc[1]={
    src.c_str()
  };

  glShaderSource(vs,1,vsSrc,nullptr);
  glCompileShader(vs);
  int compileStatus;
  glGetShaderiv(vs,GL_COMPILE_STATUS,&compileStatus);
  if(compileStatus != GL_TRUE){
    uint32_t const msgLen = 1<<11;
    char msg[msgLen];
    glGetShaderInfoLog(vs,msgLen,nullptr,msg);
    error(shaderTypeToName(type)+" shader compilation error",msg);
  }
  return vs;
}

GLuint createProgram(GLuint vs,GLuint fs){
  GLuint prg = glCreateProgram();
  glAttachShader(prg,vs);
  glAttachShader(prg,fs);

  glLinkProgram(prg);
  GLint linkStatus;
  glGetProgramiv(prg,GL_LINK_STATUS,&linkStatus);
  if(linkStatus != GL_TRUE){
    uint32_t const msgLen = 1<<11;
    char msg[msgLen];
    glGetProgramInfoLog(prg,msgLen,nullptr,msg);
    error("program linking error",msg);
  }
  return prg;
}

struct Frame{
  std::vector<uint8_t>data;
  uint32_t width = 0;
  uint32_t height = 0;
  uint32_t channels = 0;
};

void flipFrame(Frame&frame){
  for(uint32_t y=0;y<frame.height/2;++y)
    for(uint32_t x=0;x<frame.width;++x){
      for(uint32_t c=0;c<frame.channels;++c){
        auto src = (y*frame.width+x)*frame.channels+c;
        auto dst = ((frame.height-1-y)*frame.width+x)*frame.channels+c;
        auto z=frame.data.at(src);
        frame.data.at(src) = frame.data.at(dst);
        frame.data.at(dst) = z;
      }
    }
}

Frame dumpFrame(SDL_Window*window){
  Frame res;

  res.channels = 3;
  SDL_GetWindowSize(window,(int*)&res.width,(int*)&res.height);
  res.data.resize(res.channels*res.width*res.height);

  glReadPixels(0,0,res.width,res.height,GL_RGB,GL_UNSIGNED_BYTE,res.data.data());
  return res;
}

void saveFrame(std::string const&file,Frame const&f){
  auto frame = f;
  flipFrame(frame);
  stbi_write_png(file.c_str(),frame.width,frame.height,3,frame.data.data(),0);
  std::cerr << "storing screenshot to: \"" << file << "\"" << std::endl;
}

Frame loadFrame(std::string const&file){
  Frame res;
  uint8_t* data = stbi_load(file.c_str(),(int*)&res.width,(int*)&res.height,(int*)&res.channels,0);
  if(!data){
    error("loadFrame","cannot load image: "+file);
    return res;
  }

  res.data.resize(res.width*res.height*res.channels);
  for(uint32_t i=0;i<res.width*res.height*res.channels;++i)
    res.data.at(i) = data[i];

  stbi_image_free(data);
  flipFrame(res);
  return res;
}

float meanSquareError(Frame const&a,Frame const&b){
  if(a.channels    != b.channels   )return 1e10;
  if(a.width       != b.width      )return 1e10;
  if(a.height      != b.height     )return 1e10;
  if(a.data.size() != b.data.size())return 1e10;

  float meanSquareError = 0;
  for (uint32_t y = 0; y < a.height; ++y)
    for (uint32_t x = 0; x < a.width; ++x){
      for (uint32_t c = 0; c < a.channels; ++c) {
        auto pix = (y*a.width+x)*a.channels+c;
        uint8_t ucol = a.data[pix];
        uint8_t gcol = b.data[pix];
        float diff = glm::abs((float)ucol - (float)gcol);
        diff *= diff;
        meanSquareError += diff;
      }
    }

  meanSquareError /= (float)(a.width * a.height * a.channels);
  return meanSquareError;
}

#ifndef CMAKE_ROOT_DIR
#define CMAKE_ROOT_DIR "."
#endif


int main(int argc,char*argv[]){
  auto args = std::make_shared<argumentViewer::ArgumentViewer>(argc,argv);
  auto windowSize          = args->geti32v  ("--window-size"     ,{500,500}                                          ,"size of the window"     );
  auto runConformanceTests = args->isPresent("--tests"                                                               ,"runs conformance tests" );
  auto groundTruthFile     = args->gets     ("--groundtruth-file",std::string(CMAKE_ROOT_DIR)+"/src/groundtruth.png" ,"ground truth file");
  auto takeScreenShot      = args->isPresent("--take-screenshot"                                                     ,"takes screenshot of app");
  auto screenShotFile      = args->gets     ("--screenshot-file" ,std::string(CMAKE_ROOT_DIR)+"/screenshot.png"      ,"screenshot file"        );
  auto shaderFile          = args->gets     ("--shader"          ,std::string(CMAKE_ROOT_DIR)+"/shaders/student.glsl","shader file"            );
  auto triangles           = args->getu32   ("--triangles"       ,10000                                              ,"number of triangles"    );
  auto version             = args->gets     ("--version"         ,"330"                                              ,"GLSL version, try \"330\", \"300 es\", \"450\", ...");
  errorCmd                 = args->isPresent("--cmd-error","prints error to console");
  auto printHelp           = args->isPresent("-h"                                                                    ,"prints help"            );
  printHelp               |= args->isPresent("--help"                                                                ,"prints help"            );

  if(printHelp || !args->validate()){
    std::cerr << args->toStr() << std::endl;
    return EXIT_SUCCESS;
  }



  SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);

  if(SDL_Init(SDL_INIT_EVERYTHING)<0)
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_Init fail: %s\n",SDL_GetError());

  SDL_Window*   window =   SDL_CreateWindow("izg_lab06",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,windowSize.at(0),windowSize.at(1),SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
  if (!window)
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_CreateWindow fail: %s\n",SDL_GetError());

  SDL_GLContext context =  SDL_GL_CreateContext(window);
  if (!context)
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_GL_CreateContext fail: %s\n",SDL_GetError());

  ge::gl::init(SDL_GL_GetProcAddress);

  basicCamera::OrbitCamera       orbitCamera      ;
  basicCamera::PerspectiveCamera perspectiveCamera;

  orbitCamera.addDistance(3.f);
  orbitCamera.addXAngle(glm::radians(30.f));
  orbitCamera.addYAngle(glm::radians(-30.f));

  perspectiveCamera.setNear(0.01f);
  perspectiveCamera.setFar(1000.f);
  perspectiveCamera.setFovy(glm::radians(90.f));
  perspectiveCamera.setAspect(1.f);
   
  float sensitivity = 0.01f;
  float zoomSpeed = 0.02f;

  glEnable(GL_DEPTH_TEST);

  auto src = loadTxtFile(shaderFile);

  auto vs  = createShader(GL_VERTEX_SHADER,"#version "+version+"\n#define VERTEX_SHADER\n"+src);
  auto fs  = createShader(GL_FRAGMENT_SHADER,"#version "+version+"\n#define FRAGMENT_SHADER\n"+src);
  auto prg = createProgram(vs,fs);

  auto viewL = glGetUniformLocation(prg,"view");
  auto projL = glGetUniformLocation(prg,"proj");
  auto resL  = glGetUniformLocation(prg,"iResolution");
  auto timeL = glGetUniformLocation(prg,"iTime");

  GLuint vao;
  glCreateVertexArrays(1,&vao);

  auto timer = Timer<float>();

  bool running=true;
  while(running){
    SDL_Event event;
    while(SDL_PollEvent(&event)){
      if(event.type==SDL_QUIT){
        running=false;
      }
      if(event.type==SDL_MOUSEMOTION){
        auto xrel=event.motion.xrel;
        auto yrel=event.motion.yrel;
        if(event.motion.state & SDL_BUTTON_LMASK){
          orbitCamera.addXAngle(yrel * sensitivity);
          orbitCamera.addYAngle(xrel * sensitivity);
        }
        if(event.motion.state & SDL_BUTTON_RMASK){
          orbitCamera.addDistance(yrel * zoomSpeed);
        }
        if(event.motion.state & SDL_BUTTON_MMASK){
          int width,height;
          SDL_GetWindowSize(window,&width,&height);
          orbitCamera.addXPosition(+orbitCamera.getDistance() * xrel /
                                    float(width) * 2.f);
          orbitCamera.addYPosition(-orbitCamera.getDistance() * yrel /
                                    float(height) * 2.f);
        }
      }
      if(event.type==SDL_KEYDOWN){
        if(event.key.keysym.sym == SDLK_r){
          std::cerr << "shader reloaded" << std::endl;
          glDeleteShader(vs);
          glDeleteShader(fs);
          glDeleteProgram(prg);
          src = loadTxtFile(shaderFile);
          vs  = createShader(GL_VERTEX_SHADER,"#version "+version+"\n#define VERTEX_SHADER\n"+src);
          fs  = createShader(GL_FRAGMENT_SHADER,"#version "+version+"\n#define FRAGMENT_SHADER\n"+src);
          prg = createProgram(vs,fs);
          viewL = glGetUniformLocation(prg,"view");
          projL = glGetUniformLocation(prg,"proj");
          resL  = glGetUniformLocation(prg,"iResolution");
          timeL = glGetUniformLocation(prg,"iTime");
        }
        if(event.key.keysym.sym == SDLK_f){
          auto flags = SDL_GetWindowFlags(window);
          if (flags & SDL_WINDOW_FULLSCREEN_DESKTOP)
            SDL_SetWindowFullscreen(window, 0);
          else
            SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
        }
      }
      if(event.type == SDL_WINDOWEVENT) {
        if(event.window.event== SDL_WINDOWEVENT_RESIZED){
          auto const&w=event.window;
          glViewport(0,0,w.data1,w.data2);
          perspectiveCamera.setAspect((float)w.data1/(float)w.data2);
        }
      }
    }

    glClearColor(0.5,0.5,1,1);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    auto viewMatrix = orbitCamera.getView();
    auto projMatrix = perspectiveCamera.getProjection();


    glUseProgram(prg);
    glBindVertexArray(vao);

    glUniformMatrix4fv(viewL,1,GL_FALSE,glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(projL,1,GL_FALSE,glm::value_ptr(projMatrix));
    glUniform1f(timeL,timer.elapsedFromStart());

    int w,h;
    SDL_GetWindowSize(window,&w,&h);
    glUniform2f(resL,w,h);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glDrawArrays(GL_TRIANGLES,0,triangles*3);

    if(runConformanceTests){
      auto testFrame = dumpFrame(window);
      auto groundTruth = loadFrame(groundTruthFile);
      auto err = meanSquareError(testFrame,groundTruth);
      float threshold = 600.f;
      std::cerr << "Mean Square Error: " << err << std::endl;
      std::cerr << "Allowed error: " << threshold << std::endl;
      std::cerr << "Final points: " << std::endl;

      size_t maxPoints = 3;

      float t = glm::clamp(1.f-err/threshold,0.f,1.f);
      std::cout << std::fixed << std::setprecision(1) << maxPoints * t << std::endl;

      running = false;
    }
    if(takeScreenShot){
      saveFrame(screenShotFile,dumpFrame(window));
      running = false;
    }
    SDL_GL_SwapWindow(window);
  }

  glDeleteProgram(prg);
  glDeleteShader(vs);
  glDeleteShader(fs);
  glDeleteVertexArrays(1,&vao);


  SDL_GL_DeleteContext(context);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
