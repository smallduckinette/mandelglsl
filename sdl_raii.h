#ifndef __MANDELGLSL_SDL_RAII_H__
#define __MANDELGLSL_SDL_RAII_H__

#include <memory>

// SDL_Window
struct SDL_WindowDeleter
{
  void operator()(SDL_Window * window)
  {
    SDL_DestroyWindow(window);
  }
};

typedef std::unique_ptr<SDL_Window, SDL_WindowDeleter> SDL_WindowRef;

// SDL_GLContext
struct SDL_GLContextDeleter
{
  void operator()(void * context)
  {
    SDL_GL_DeleteContext(context);
  }  
};

typedef std::unique_ptr<void, SDL_GLContextDeleter> SDL_GLContextRef;

// SDL_Renderer
struct SDL_RendererDeleter
{
  void operator()(SDL_Renderer * renderer)
  {
    SDL_DestroyRenderer(renderer);
  }  
};

typedef std::unique_ptr<SDL_Renderer, SDL_RendererDeleter> SDL_RendererRef;

#endif
