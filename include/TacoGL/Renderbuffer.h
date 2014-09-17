#ifndef __TACOGL_RENDERBUFFER__
#define __TACOGL_RENDERBUFFER__

#include <TacoGL/OpenGL.h>
#include <TacoGL/Error.h>
#include <TacoGL/Object.h>

namespace TacoGL
{

  class Renderbuffer : public Object
  {
  public:
    Renderbuffer();
    ~Renderbuffer();

    void bind();
    void unbind();

    void allocate(gl::GLenum internalFormat, size_t width, size_t height);
  };

} // end namespace TacoGL

#endif
