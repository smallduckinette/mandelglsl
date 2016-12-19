#include <stdexcept>
#include <iostream>
#include <vector>
#include <fstream>
#include <streambuf>

#include <SDL2/SDL.h>
#include <GL/glew.h>

#include "mandelbrotapp.h"

int main()
{
  SDL_Init( SDL_INIT_EVERYTHING );
  try
  {
    MandelbrotApp app;
    app.run();
  }
  catch(const std::exception & e)
  {
    std::cout << e.what() << std::endl;
  }
  
  SDL_Quit();
  return 0;
}
