#ifndef __MANDELGLSL_RAII_H__
#define __MANDELGLSL_RAII_H__

#include <memory>

template<void (*func)(GLuint)>
struct CustomDeleter
{
  struct pointer
  {
    GLuint _val;
    pointer(std::nullptr_t = nullptr) : _val(0) {}
    pointer(GLuint val) : _val(val) {}
    operator GLuint() const { return _val; }
    friend bool operator == (pointer x, pointer y) { return x._val == y._val; }
    friend bool operator != (pointer x, pointer y) { return x._val != y._val; }
  };
  
  void operator()(GLuint ref)
  {
    func(ref);
  }
};

inline void doDeleteShader(GLuint ref)
{
  glDeleteShader(ref);
}

inline void doDeleteProgram(GLuint ref)
{
  glDeleteProgram(ref);
}

typedef std::unique_ptr<GLuint, CustomDeleter<doDeleteShader> > ShaderRef;
typedef std::unique_ptr<GLuint, CustomDeleter<doDeleteProgram> > ProgramRef;

#endif
