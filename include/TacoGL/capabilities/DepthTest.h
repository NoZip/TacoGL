#ifndef __TACOGL_DEPTH_TEST__
#define __TACOGL_DEPTH_TEST__

#include <TacoGL/OpenGL.h>
#include <TacoGL/Error.h>
#include <TacoGL/get.h>

#include <TacoGL/capabilities/Capability.h>

namespace TacoGL
{

  class DepthTest : public Capability<gl::GL_DEPTH_TEST>
  {
  public:
    static gl::GLenum getFunction()
    {
      return static_cast<gl::GLenum>(get<gl::GL_DEPTH_FUNC, gl::GLint>());
    }

    static void setFunction(gl::GLenum function)
    {
      gl::glDepthFunc(function);
    }
  };

} // end namespace GL

#endif
