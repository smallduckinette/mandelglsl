#ifndef __MANDELGLSL_MANDELBROTAPP_H__
#define __MANDELGLSL_MANDELBROTAPP_H__

#include <vector>

#include "sdlapp.h"
#include "program.h"

class MandelbrotApp : public SdlApp
{
public:
  MandelbrotApp();

  void onKeyDown(const SDL_KeyboardEvent & key) override;
  void onMouseWheel(const SDL_MouseWheelEvent & wheel) override;
  void onMouseMotion(const SDL_MouseMotionEvent & motion) override;
  void onDraw() override;
  
private:
  struct Vertex
  {
    float Position[3];
  };
  
  // State
  SDL_GLContextRef _context;
  SDL_RendererRef _renderer;
  std::unique_ptr<Program> _program;

  // Locations
  GLuint _v_s;
  GLuint _v_v;
  GLuint _v_zoom;
  GLuint _v_pos;

  // Variables
  float _cs;
  float _cv;
  float _zoom;
  float _posx;
  float _posy;

  // Geometry
  BufferRef _vertexBuffer;
  BufferRef _indexBuffer;
};

#endif
