#include <cassert>

#include <TacoGL/Texture.h>

#include <TacoGL/Sampler.h>

using namespace gl;
using namespace TacoGL;

Sampler::Sampler()
{
  glGenSamplers(1, &m_id);
}

Sampler::~Sampler()
{
  glDeleteSamplers(1, &m_id);
}

void Sampler::bind(size_t unit)
{
  assert(unit < Texture::getTextureUnitCount());
  glBindSampler(unit, m_id);
}

namespace
{
  inline void _getParameter(GLuint samplerId, GLenum parameter, GLfloat *value)
  {
    glGetSamplerParameterfv(samplerId, parameter, value);
  }

  inline void _getParameter(GLuint samplerId, GLenum parameter, GLint *value)
  {
    glGetSamplerParameterIiv(samplerId, parameter, value);
  }

  inline void _getParameter(GLuint samplerId, GLenum parameter, GLuint *value)
  {
    glGetSamplerParameterIuiv(samplerId, parameter, value);
  }

  inline void _getParameter(GLuint samplerId, GLenum parameter, GLenum *value)
  {
    GLuint uvalue;
    glGetSamplerParameterIuiv(samplerId, parameter, &uvalue);
    *value = static_cast<GLenum>(uvalue);
  }

  template <GLenum PARAMETER, typename T>
  inline T getParameter(GLuint samplerId)
  {
    T value;
    _getParameter(samplerId, PARAMETER, &value);
    return value;
  }

  template <GLenum PARAMETER, typename T>
  inline T getParameterv(GLuint samplerId)
  {
    T value;
    _getParameter(samplerId, PARAMETER, value.data());
    return value;
  }
}

namespace
{
  inline void _setParameter(GLuint samplerId, GLenum parameter, const GLfloat *value)
  {
    glSamplerParameterfv(samplerId, parameter, value);
  }

  inline void _setParameter(GLuint samplerId, GLenum parameter, const GLint *value)
  {
    glSamplerParameterIiv(samplerId, parameter, value);
  }

  inline void _setParameter(GLuint samplerId, GLenum parameter, const GLuint *value)
  {
    glSamplerParameterIuiv(samplerId, parameter, value);
  }

  inline void _setParameter(GLuint samplerId, GLenum parameter, const GLenum *value)
  {
    GLuint uvalue = static_cast<GLuint>(*value);
    glSamplerParameterIuiv(samplerId, parameter, &uvalue);
  }

  template <GLenum PARAMETER, typename T>
  inline void setParameter(GLuint samplerId, const T &value)
  {
    _setParameter(samplerId, PARAMETER, &value);
  }

  template <GLenum PARAMETER, typename T>
  inline void setParameterv(GLuint samplerId, const T &value)
  {
    _setParameter(samplerId, PARAMETER, value.data());
  }
}

GLenum Sampler::getMagFilter() const
{
  return getParameter<GL_TEXTURE_MAG_FILTER, GLenum>(m_id);
}

GLenum Sampler::getMinFilter() const
{
  return getParameter<GL_TEXTURE_MIN_FILTER, GLenum>(m_id);
}

size_t Sampler::getMinLOD() const
{
  return getParameter<GL_TEXTURE_MIN_LOD, GLuint>(m_id);
}

size_t Sampler::getMaxLOD() const
{
  return getParameter<GL_TEXTURE_MAX_LOD, GLuint>(m_id);
}

// TODO:TEXTURE_WRAP, Vector or scalar ?

Vector4 Sampler::getBorderColor() const {
  return getParameterv<GL_TEXTURE_BORDER_COLOR, Vector4>(m_id);
}

// TODO: TEXTURE_COMPARE_MODE

GLenum Sampler::getCompareFunction() const
{
  return getParameter<GL_TEXTURE_COMPARE_FUNC, GLenum>(m_id);
}

void Sampler::setMagFilter(GLenum value)
{
  setParameter<GL_TEXTURE_MAG_FILTER, GLenum>(m_id, value);
}

void Sampler::setMinFilter(GLenum value)
{
  setParameter<GL_TEXTURE_MIN_FILTER, GLenum>(m_id, value);
}

void Sampler::setMinLOD(size_t value)
{
  setParameter<GL_TEXTURE_MIN_LOD, GLuint>(m_id, value);
}

void Sampler::setMaxLOD(size_t value)
{
  setParameter<GL_TEXTURE_MAX_LOD, GLuint>(m_id, value);
}

// TODO:TEXTURE_WRAP, Vector or scalar ?

void Sampler::setBorderColor(const Vector4 &value)
{
  setParameterv<GL_TEXTURE_BORDER_COLOR, Vector4>(m_id, value);
}

// TODO: TEXTURE_COMPARE_MODE

void Sampler::setCompareFunction(GLenum value)
{
  setParameter<GL_TEXTURE_COMPARE_FUNC, GLenum>(m_id, value);
}
