#ifndef __TACOGL_GET__
#define __TACOGL_GET__

#include <cstddef>

#include <TacoGL/OpenGL.h>
#include <TacoGL/Error.h>
#include <TacoGL/algebra.h>

namespace TacoGL
{

  namespace {
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
  }

  template<gl::GLenum PARAMETER, typename T>
  inline T get()
  {
    T data;
    _get(PARAMETER, &data);
    return data;
  }

  template<gl::GLenum PARAMETER, typename T>
  T getv()
  {
    T vector;
    auto it = vector.data();
    for (int i = 0; i < vector.size(); ++i)
    {
      _get(PARAMETER, i, it++);
    }
    return vector;
  }

  inline gl::GLuint getActiveTexture()
  {
    return get<gl::GL_ACTIVE_TEXTURE, gl::GLint>() - static_cast<gl::GLint>(gl::GL_TEXTURE0);
  }

  inline Vector2 getAliasedLineWidthRange()
  {
    return getv<gl::GL_ALIASED_LINE_WIDTH_RANGE, Vector2>();
  }

  inline gl::GLuint getArrayBufferBinding()
  {
    return get<gl::GL_ARRAY_BUFFER_BINDING, gl::GLint>();
  }

  inline Vector4 getColorClearValue()
  {
    return getv<gl::GL_COLOR_CLEAR_VALUE, Vector4>();
  }

  inline Vector4b getColorWriteMask()
  {
    return getv<gl::GL_COLOR_WRITEMASK, Vector4b>();
  }

// TODO : gl::GL_COMPRESSED_TEXTURE_FORMATS

  inline size_t getMaxCombinedTextureImageUnits()
  {
    return get<gl::GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, gl::GLint>();
  }

  inline Vector3i getMaxComputeWorkGroupCount()
  {
    return getv<gl::GL_MAX_COMPUTE_WORK_GROUP_COUNT, Vector3i>();
  }

  inline Vector3i getMaxComputeWorkGroupSize()
  {
    return getv<gl::GL_MAX_COMPUTE_WORK_GROUP_SIZE, Vector3i>();
  }

} // end namespace GL

#endif