#include "program.h"

#include <vector>
#include <iostream>

Program::Program(std::istream & vertexStream,
                 std::istream & fragmentStream):
  _vertexShader(glCreateShader(GL_VERTEX_SHADER)),
  _fragmentShader(glCreateShader(GL_FRAGMENT_SHADER)),
  _program(glCreateProgram())
{
  loadShader(_vertexShader.get(), vertexStream);
  loadShader(_fragmentShader.get(), fragmentStream);
  glAttachShader(_program.get(), _vertexShader.get());
  glAttachShader(_program.get(), _fragmentShader.get());
  glLinkProgram(_program.get());
  printLinkStatus(_program.get());
}

void Program::use()
{
  glUseProgram(_program.get());
}

GLuint Program::getAttribLocation(const char * attribute) const
{
  return glGetAttribLocation(_program.get(), attribute);
}

GLuint Program::getUniformLocation(const char * attribute) const
{
  return glGetUniformLocation(_program.get(), attribute);
}

void Program::loadShader(GLuint shader, std::istream & stream) const
{
  std::string source((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
  const char * sourcePtr = source.c_str();
  glShaderSource(shader, 1, &sourcePtr, nullptr);
  glCompileShader(shader);
  printCompileStatus(shader);
}

void Program::printCompileStatus(GLuint shader) const
{
  GLint blen = 0;	
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH , &blen);       
  
  if (blen > 1)
  {
    std::vector<GLchar> compilerLog(blen);
    
    GLsizei slen = 0;
    glGetInfoLogARB(shader, blen, &slen, &compilerLog[0]);
    std::cout << "compiler_log:" << &compilerLog[0] << std::endl;
  }
}

void Program::printLinkStatus(GLuint shader) const
{
  GLint blen = 0;	
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH , &blen);       
  
  if (blen > 1)
  {
    std::vector<GLchar> compilerLog(blen);
    
    GLsizei slen = 0;
    glGetInfoLogARB(shader, blen, &slen, &compilerLog[0]);
    std::cout << "compiler_log:" << &compilerLog[0] << std::endl;
  }
}
