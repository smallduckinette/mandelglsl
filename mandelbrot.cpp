#include <stdexcept>
#include <iostream>
#include <vector>
#include <fstream>
#include <streambuf>

#include <SDL2/SDL.h>
#include <GL/glew.h>

#include "program.h"

int main()
{
  SDL_Init( SDL_INIT_EVERYTHING );
  try
  {    
    SDL_Window *window = SDL_CreateWindow("Mandelbrot",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          800,
                                          800,
                                          SDL_WINDOW_OPENGL);
    
    if(!window)
      throw std::runtime_error(std::string("Could not create window : ") + SDL_GetError());
    
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    
    SDL_GL_CreateContext(window);
    SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
    SDL_GL_SetSwapInterval(1);

    glewInit();

    glViewport(0, 0, 800, 800);
    
    std::ifstream vertexSource("vertex.txt");
    std::ifstream fragmentSource("frag.txt");
    Program program(vertexSource, fragmentSource);
    
    program.use();
    
    GLuint positionSlot = program.getAttribLocation("Position");
    glEnableVertexAttribArray(positionSlot);
    
    GLuint color_s = program.getUniformLocation("color_s");
    GLuint color_v = program.getUniformLocation("color_v");
    
    float cs = 0.8;
    float cv = 0.8;
    
    glUniform1f(color_s, cs);
    glUniform1f(color_v, cv);
    
    typedef struct {
      float Position[3];
    } Vertex;
    
    const Vertex vertices[] = {
      {{1, -1, 0}},
      {{1, 1, 0}},
      {{-1, 1, 0}},
      {{-1, -1, 0}}
    };
    
    const GLubyte indices[] = {
      0, 1, 2,
      2, 3, 0
    };
    
    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    GLuint indexBuffer;
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(positionSlot, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    
    bool done = false;
    unsigned int lastTime = SDL_GetTicks();
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
          if(event.key.keysym.sym == SDLK_s)
          {
            if(event.key.keysym.mod & KMOD_SHIFT)
            {
              cs -= 0.05;
            }
            else
            {
              cs += 0.05;
            }
            cs = std::max(std::min(cs, 1.0f), 0.0f);
            glUniform1f(color_s, cs);
          }
          else if(event.key.keysym.sym == SDLK_v)
          {
            if(event.key.keysym.mod & KMOD_SHIFT)
            {
              cv -= 0.05;
            }
            else
            {
              cv += 0.05;
            }
            cv = std::max(std::min(cv, 1.0f), 0.0f);
            glUniform1f(color_v, cv);
          }
          break;
        }
      }
      
      if(done)
        continue;
      
      // Draw the frame
      glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(indices[0]), GL_UNSIGNED_BYTE, 0);
      
      SDL_GL_SwapWindow(window);
      unsigned int currentTime = SDL_GetTicks();
      std::cout << (currentTime - lastTime) << std::endl;
      lastTime = currentTime;
    }
  }
  catch(const std::exception & e)
  {
    std::cout << e.what() << std::endl;
  }
  
  SDL_Quit();
  return 0;
}
