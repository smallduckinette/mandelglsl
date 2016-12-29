#include "mandelbrotapp.h"

#include <fstream>
#include <GL/glew.h>

#include "program.h"

MandelbrotApp::MandelbrotApp():
  SdlApp("Mandelbrot"),
  _cs(0.8),
  _cv(0.8),
  _zoom(1.0),
  _posx(0.25),
  _posy(0.0)
{
  // Setup GL options
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetSwapInterval(1);

  // Create context and renderer
  _context.reset(SDL_GL_CreateContext(_window.get()));
  _renderer.reset(SDL_CreateRenderer(_window.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE));

  // Load GL functions
  glewInit();

  // Create the viewport
  glViewport(0, 0, _w, _h);

  // Load the shader
  std::ifstream vertexSource("vertex.txt");
  std::ifstream fragmentSource("frag.txt");
  _program = std::make_unique<Program>(vertexSource, fragmentSource);
  _program->use();
  
  GLuint positionSlot = _program->getAttribLocation("Position");
  glEnableVertexAttribArray(positionSlot);
  
  _v_s = _program->getUniformLocation("color_s");
  _v_v = _program->getUniformLocation("color_v");
  _v_zoom = _program->getUniformLocation("zoom");
  _v_pos = _program->getUniformLocation("position");

  glUniform1f(_v_s, _cs);
  glUniform1f(_v_v, _cv);
  glUniform1f(_v_zoom, _zoom);
  glUniform2f(_v_pos, _posx, _posy);
  
  // Stretch factor
  float stretchFactor = (float)_w / (float)_h;
  GLuint v_stretch = _program->getUniformLocation("stretch");
  glUniform1f(v_stretch, stretchFactor);

  // Load the geometry
  std::vector<Vertex> vertices({
      {{1, -1, 0}},
      {{1, 1, 0}},
      {{-1, 1, 0}},
      {{-1, -1, 0}}
    });
  std::vector<GLubyte> indices({0, 1, 2, 2, 3, 0});
  
  _vertexBuffer.reset(createBuffer());
  glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer.get());
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
  
  _indexBuffer.reset(createBuffer());
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer.get());
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte) * indices.size(), &indices[0], GL_STATIC_DRAW);
  
  glVertexAttribPointer(positionSlot, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
}

void MandelbrotApp::onKeyDown(const SDL_KeyboardEvent & key)
{
  if(key.keysym.sym == SDLK_s)
  {
    if(key.keysym.mod & KMOD_SHIFT)
    {
      _cs -= 0.05;
    }
    else
    {
      _cs += 0.05;
    }
    _cs = std::max(std::min(_cs, 1.0f), 0.0f);
    glUniform1f(_v_s, _cs);
  }
  else if(key.keysym.sym == SDLK_v)
  {
    if(key.keysym.mod & KMOD_SHIFT)
    {
      _cv -= 0.05;
    }
    else
    {
      _cv += 0.05;
    }
    _cv = std::max(std::min(_cv, 1.0f), 0.0f);
    glUniform1f(_v_v, _cv);
  }
}

void MandelbrotApp::onMouseWheel(const SDL_MouseWheelEvent & wheel)
{
  if(wheel.y < 0)
    _zoom *= 1.5f;
  else
    _zoom /= 1.5f;
  glUniform1f(_v_zoom, _zoom);
}

void MandelbrotApp::onMouseMotion(const SDL_MouseMotionEvent & motion)
{
  if(motion.state & SDL_BUTTON_LMASK)
  {
    _posx -= (motion.xrel * _zoom / (_w / 2.0f));
    _posy += (motion.yrel * _zoom / (_h / 2.0f));
    glUniform2f(_v_pos, _posx, _posy);
  }
}

void MandelbrotApp::onDraw()
{
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
}
