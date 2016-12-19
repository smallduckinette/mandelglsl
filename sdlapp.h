#ifndef __MANDELGLSL_SDLAPP_H__
#define __MANDELGLSL_SDLAPP_H__

#include <string>

#include <SDL2/SDL.h>

#include "sdl_raii.h"

class SdlApp
{
public:
  SdlApp(const std::string & appName);
  
  void run();

  virtual void onKeyDown(const SDL_KeyboardEvent &) {};
  virtual void onMouseWheel(const SDL_MouseWheelEvent &) {};
  virtual void onMouseMotion(const SDL_MouseMotionEvent &) {};
  virtual void onDraw() = 0;
  
protected:
  SDL_WindowRef _window;
};

#endif
