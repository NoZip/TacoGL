#ifndef __TACOGL_GET__
#define __TACOGL_GET__

#include <cstddef>

#include <TacoGL/OpenGL.h>
#include <TacoGL/Error.h>
#include <TacoGL/algebra.h>

namespace TacoGL
{

  namespace {

    //==========================//
    // Getters for OpenGL types //
    //==========================//

    inline void _get(gl::GLenum parameter, gl::GLboolean *data)
    {
      gl::glGetBooleanv(parameter, data);
    }

    inline void _get(gl::GLenum parameter, gl::GLdouble *data)
    {
      gl::glGetDoublev(parameter, data);
    }

    inline void _get(gl::GLenum parameter, gl::GLfloat *data)
    {
      gl::glGetFloatv(parameter, data);
    }

    inline void _get(gl::GLenum parameter, gl::GLint *data)
    {
      gl::glGetIntegerv(parameter, data);
    }

    //------------------//
    // Indexed versions //
    //------------------//

    inline void _get(gl::GLenum parameter, gl::GLuint index, gl::GLboolean *data)
    {
      gl::glGetBooleani_v(parameter, index, data);
    }

    inline void _get(gl::GLenum parameter, gl::GLuint index, gl::GLdouble *data)
    {
      gl::glGetDoublei_v(parameter, index, data);
    }

    inline void _get(gl::GLenum parameter, gl::GLuint index, gl::GLfloat *data)
    {
      gl::glGetFloati_v(parameter, index, data);
    }

    inline void _get(gl::GLenum parameter, gl::GLuint index, gl::GLint *data)
    {
      gl::glGetIntegeri_v(parameter, index, data);
    }

    //==========================//
    // Getters for others types //
    //==========================//

    inline void _get(gl::GLenum parameter, gl::GLuint *data)
    {
      gl::GLint value;
      _get(parameter, &value);
      *data = static_cast<gl::GLuint>(value);
    }

    inline void _get(gl::GLenum parameter, bool *data)
    {
      gl::GLboolean value;
      _get(parameter, &value);
      *data = (value == gl::GL_TRUE);
    }

    inline void _get(gl::GLenum parameter, size_t *data)
    {
      gl::GLint value;
      _get(parameter, &value);
      *data = static_cast<size_t>(value);
    }

    //------------------//
    // Indexed versions //
    //------------------//

    inline void _get(gl::GLenum parameter, gl::GLuint index, bool *data)
    {
      gl::GLboolean value;
      _get(parameter, index, &value);
      *data = (value == gl::GL_TRUE);
    }

    inline void _get(gl::GLenum parameter, gl::GLuint index, size_t *data)
    {
      gl::GLint value;
      _get(parameter, index, &value);
      *data = static_cast<size_t>(value);
    }
  }

  /**
   * Fetches OpenGL global parameters.
   *
   * @tparam PARAMETER The parameter to fetch.
   * @tparam Type      The type to return.
   */
  template<gl::GLenum PARAMETER, typename Type>
  inline Type get()
  {
    Type data;
    _get(PARAMETER, &data);
    return data;
  }

  /**
   * Fetches OpenGL global parameters, Vector version.
   *
   * @tparam PARAMETER The parameter to fetch.
   * @tparam Vector    The type to return.
   */
  template<gl::GLenum PARAMETER, typename Vector>
  inline Vector getv()
  {
    Vector vector;

    auto it = vector.data();

    for (int i = 0; i < vector.size(); ++i)
    {
      _get(PARAMETER, i, it++);
    }

    return vector;
  }

} // end namespace GL

#endif