#include <stdexcept>
#include <iostream>
#include <vector>
#include <fstream>
#include <streambuf>

#include <SDL2/SDL.h>
#include <GL/glew.h>

const char * vertexShaderSource =
  "attribute vec4 Position;\n"
  "attribute vec4 SourceColor;\n"
  " \n"
  "varying vec2 c;\n"
  " \n"
  "void main(void) {\n"
  "    gl_Position = Position;\n"
  "    c = gl_Position.xy;\n"
  "}\n";

const char * fragmentShaderSource =
  "varying vec2 c; \n"
  "\n"
  "vec2 c_mult(vec2 a, vec2 b) {\n"
  "  return vec2(a.x * b.x - a.y * b.y,\n"
  "              (a.x+a.y)*(b.x+b.y) - a.x*b.x - a.y*b.y);\n"
  "}  \n"
  "float mandel(int depth, vec2 c) {\n"
  "  return 1.0;\n"
  "}  \n"
  "void main(void) {\n"
  "    gl_FragColor = mandel(100, c) * vec4(1.0, 1.0, 1.0, 1.0);\n"
  "}\n";

void printCompileStatus(GLuint shader)
{
  GLint blen = 0;	
  GLsizei slen = 0;
  
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH , &blen);       
  
  if (blen > 1)
  {
    std::vector<GLchar> compilerLog(blen);
    
    glGetInfoLogARB(shader, blen, &slen, &compilerLog[0]);
    std::cout << "compiler_log:" << &compilerLog[0] << std::endl;
  }
}

void printLinkStatus(GLuint shader)
{
  GLint blen = 0;	
  GLsizei slen = 0;
  
  glGetProgramiv(shader, GL_INFO_LOG_LENGTH , &blen);       
  
  if (blen > 1)
  {
    std::vector<GLchar> compilerLog(blen);
    
    glGetInfoLogARB(shader, blen, &slen, &compilerLog[0]);
    std::cout << "linker_log:" << &compilerLog[0] << std::endl;
  }
}

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

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    
    std::ifstream frag("frag.txt");
    std::string fragSource((std::istreambuf_iterator<char>(frag)), std::istreambuf_iterator<char>());
    const char * fragSourcePtr = fragSource.c_str();
    
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glShaderSource(fragmentShader, 1, &fragSourcePtr, nullptr);
    
    glCompileShader(vertexShader);
    printCompileStatus(vertexShader);
    
    glCompileShader(fragmentShader);
    printCompileStatus(fragmentShader);
  
    GLuint programObject = glCreateProgram();
    glAttachShader(programObject, vertexShader);
    glAttachShader(programObject, fragmentShader);
    glLinkProgram(programObject);
    printLinkStatus(programObject);
    

    glUseProgram(programObject);
    
    GLuint positionSlot = glGetAttribLocation(programObject, "Position");
    GLuint colorSlot = glGetAttribLocation(programObject, "SourceColor");
    glEnableVertexAttribArray(positionSlot);
    glEnableVertexAttribArray(colorSlot);
    
    typedef struct {
      float Position[3];
      float Color[4];
    } Vertex;
    
    const Vertex vertices[] = {
      {{1, -1, 0}, {1, 0, 0, 1}},
      {{1, 1, 0}, {0, 1, 0, 1}},
      {{-1, 1, 0}, {0, 0, 1, 1}},
      {{-1, -1, 0}, {0, 0, 0, 1}}
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
        }
      }
      
      if(done)
        continue;
      
      // Draw the frame
      glClearColor(1., 0., 1., 1.);
      glClear(GL_COLOR_BUFFER_BIT);
      glViewport(0, 0, 800, 800);
      glVertexAttribPointer(positionSlot, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
      glVertexAttribPointer(colorSlot, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) (sizeof(float) * 3));
      
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
