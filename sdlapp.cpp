#include "sdlapp.h"

#include <stdexcept>

SdlApp::SdlApp(const std::string & appName):
  _window(SDL_CreateWindow(appName.c_str(),
                           SDL_WINDOWPOS_UNDEFINED,
                           SDL_WINDOWPOS_UNDEFINED,
                           800,
                           800,
                           SDL_WINDOW_OPENGL))
{
  if(!_window)
    throw std::runtime_error(std::string("Could not create window : ") + SDL_GetError());
}

void SdlApp::run()
{
  bool done = false;
  while(!done)
  {
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
      switch (event.type)
      {
      case SDL_QUIT:
        done = true;
        break;
      case SDL_KEYDOWN:
        onKeyDown(event.key);
        break;
      case SDL_MOUSEWHEEL:
        onMouseWheel(event.wheel);
        break;
      case SDL_MOUSEMOTION:
        onMouseMotion(event.motion);
        break;
      }
    }
    if(done)
      continue;
    onDraw();
    SDL_GL_SwapWindow(_window.get());
  }
}
