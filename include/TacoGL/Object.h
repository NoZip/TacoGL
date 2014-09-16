#ifndef __TacoGL_OBJECT__
#define __TacoGL_OBJECT__

#include <TacoGL/OpenGL.h>

namespace TacoGL
{

  class Object
  {
  public:
    Object() : m_id(0) {}
    virtual ~Object() {}

    gl::GLuint getId() const { return m_id; }

  protected:
    gl::GLuint m_id;
  };

} // end namespace GL

#endif
