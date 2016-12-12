#ifndef __MANDELGLSL_PROGRAM_H__
#define __MANDELGLSL_PROGRAM_H__

#include <istream>
#include <GL/glew.h>

#include "raii.h"

class Program
{
public:
  Program(std::istream & vertexStream,
          std::istream & fragmentStream);
  
  void use();
  GLuint getAttribLocation(const char * attribute) const;
  
private:
  void loadShader(GLuint shader, std::istream & stream) const;
  void printCompileStatus(GLuint shader) const;
  void printLinkStatus(GLuint shader) const;
  
  ShaderRef _vertexShader;
  ShaderRef _fragmentShader;
  ProgramRef _program;
};

#endif
