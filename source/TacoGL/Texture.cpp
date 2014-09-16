#include <cassert>
#include <algorithm>

#include <SOIL/SOIL.h>

#include <TacoGL/get.h>

#include <TacoGL/Texture.h>

using namespace gl;
using namespace TacoGL;

//==============//
// Unit Manager //
//==============//

Texture::UnitManager::UnitManager()
: m_unitUsage(Texture::getTextureUnitCount(), true), m_unitBinding()
{
  
}

Texture::UnitManager::~UnitManager()
{

}

bool Texture::UnitManager::isAvaible(size_t unit) const
{
  return m_unitUsage.at(unit);
}

bool Texture::UnitManager::isBinded(gl::GLuint textureId) const
{
  return (m_unitBinding.find(textureId) != m_unitBinding.end());
}

size_t Texture::UnitManager::getUnitBinding(gl::GLuint textureId) const
{
  return m_unitBinding.at(textureId).first;
}

gl::GLenum Texture::UnitManager::getTargetBinding(gl::GLuint textureId) const
{
  return m_unitBinding.at(textureId).second;
}

void Texture::UnitManager::bind(size_t unit, GLenum target, GLuint textureId, GLuint samplerId)
{
  if (isBinded(textureId))
  {
    if (getUnitBinding(textureId) == unit)
      return;
    else
      unbind(textureId);
  }

  assert(isAvaible(unit));

  Texture::setActiveTextureUnit(unit);

  glBindTexture(target, textureId);

  if (samplerId)
  {
    glBindSampler(unit, samplerId);
  }

  m_unitUsage.at(unit) = false;
  m_unitBinding.emplace(textureId, BindingPair{unit, target});
}

size_t Texture::UnitManager::bind(GLenum target, GLuint textureId, GLuint samplerId)
{
  auto firstAvaible = std::find(m_unitUsage.begin(), m_unitUsage.end(), true);

  size_t unit = firstAvaible - m_unitUsage.begin();

  bind(unit, target, textureId, samplerId);

  return unit;
}

void Texture::UnitManager::unbind(GLuint textureId)
{
  assert(isBinded(textureId));

  size_t unit = getUnitBinding(textureId);
  GLenum target = getTargetBinding(textureId);

  assert(!isAvaible(unit));

  Texture::setActiveTextureUnit(unit);

  glBindTexture(target, 0);
  glBindSampler(unit, 0);

  m_unitUsage.at(unit) = true;
  m_unitBinding.erase(textureId);
}

void Texture::UnitManager::unbindAll()
{
  std::fill(m_unitUsage.begin(), m_unitUsage.end(), true);
  m_unitBinding.clear();
}

//=========//
// Texture //
//=========//

size_t Texture::getTextureUnitCount()
{
  return get<GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, GLint>();
}

size_t Texture::getActiveTextureUnit()
{
  return get<GL_ACTIVE_TEXTURE, GLint>() - static_cast<GLint>(GL_TEXTURE0);
}

void Texture::setActiveTextureUnit(size_t unit)
{
  assert(unit < getTextureUnitCount());

  glActiveTexture(GL_TEXTURE0 + unit);
}

Texture::UnitManager *Texture::s_manager = nullptr;

Texture::UnitManager & Texture::getUnitManager()
{
  if (!s_manager)
    s_manager = new UnitManager();

  return *s_manager;
}

Texture::Texture() : m_sampler(nullptr)
{
  glGenTextures(1, &m_id);
}

Texture::~Texture()
{
  glDeleteTextures(1, &m_id);
}

//-----------------//
// Texture Binding //
//-----------------//

void Texture::bind(size_t unit, GLenum target)
{
  getUnitManager().bind(unit, target, m_id, (m_sampler) ? m_sampler->getId() : 0);
}

size_t Texture::bind(GLenum target)
{
  return getUnitManager().bind(target, m_id, (m_sampler) ? m_sampler->getId() : 0);
}

void Texture::unbind()
{
  getUnitManager().unbind(m_id);
}

void Texture::unbindAll()
{
  getUnitManager().unbindAll();
}

void Texture::setData(
  size_t level,
  gl::GLenum format,
  gl::GLenum internalFormat,
  gl::GLenum type,
  size_t size,
  void *data
)
{
  glTexImage1D(
    getUnitManager().getTargetBinding(m_id),
    level,
    static_cast<GLint>(internalFormat),
    size,
    0,
    format,
    type,
    data
  );
}

void Texture::setData(
  size_t level,
  gl::GLenum format,
  gl::GLenum internalFormat,
  gl::GLenum type,
  size_t width, size_t height,
  void *data
)
{
  glTexImage2D(
    getUnitManager().getTargetBinding(m_id),
    level,
    static_cast<GLint>(internalFormat),
    width, height,
    0,
    format,
    type,
    data
  );
}

void Texture::setData(
  size_t level,
  gl::GLenum format,
  gl::GLenum internalFormat,
  gl::GLenum type,
  size_t width, size_t height, size_t depth,
  void *data
)
{
  glTexImage3D(
    getUnitManager().getTargetBinding(m_id),
    level,
    static_cast<GLint>(internalFormat),
    width, height, depth,
    0,
    format,
    type,
    data
  );
}

void Texture::generateMipmaps()
{
  glGenerateMipmap(getUnitManager().getTargetBinding(m_id));
}

void Texture::load(const char *filename)
{
  Vector2i size;
  int channels;
  unsigned char *data = SOIL_load_image(filename, &(size[0]), &(size[1]), &channels, SOIL_LOAD_RGB);

  setData(0, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, size[0], size[1], (void*) data);
}

//====================//
// Texture Parameters //
//====================//

//---------//
// Getters //
//---------//

namespace 
{
  inline void _getParameter(GLenum target, GLenum parameter, GLfloat *value)
  {
    glGetTexParameterfv(target, parameter, value);
  }

  inline void _getParameter(GLenum target, GLenum parameter, GLint *value)
  {
    glGetTexParameterIiv(target, parameter, value);
  }

  inline void _getParameter(GLenum target, GLenum parameter, GLuint *value)
  {
    glGetTexParameterIuiv(target, parameter, value);
  }

  inline void _getParameter(GLenum target, GLenum parameter, GLenum *value)
  {
    GLuint uvalue;
    glGetTexParameterIuiv(target, parameter, &uvalue);
    *value = static_cast<GLenum>(uvalue);
  }

  template <GLenum PARAMETER, typename T>
  inline T getParameter(GLenum target)
  {
    T value;
    _getParameter(target, PARAMETER, &value);
    return value;
  }

  template <GLenum PARAMETER, typename T>
  inline T getParameterv(GLenum target)
  {
    T value;
    _getParameter(target, PARAMETER, value.data());
    return value;
  }
}

//---------//
// Setters //
//---------//

namespace
{
  inline void _setParameter(GLenum target, GLenum parameter, const GLfloat *value)
  {
    glTexParameterfv(target, parameter, value);
  }

  inline void _setParameter(GLenum target, GLenum parameter, const GLint *value)
  {
    glTexParameterIiv(target, parameter, value);
  }

  inline void _setParameter(GLenum target, GLenum parameter, const GLuint *value)
  {
    glTexParameterIuiv(target, parameter, value);
  }

  inline void _setParameter(GLenum target, GLenum parameter, const GLenum *value)
  {
    GLuint uvalue = static_cast<GLuint>(*value);
    glTexParameterIuiv(target, parameter, &uvalue);
  }

  template <GLenum PARAMETER, typename T>
  inline void setParameter(GLenum target, const T &value)
  {
    _setParameter(target, PARAMETER, &value);
  }

  template <GLenum PARAMETER, typename T>
  inline void setParameterv(GLenum target, const T &value)
  {
    _setParameter(target, PARAMETER, value.data());
  }
}

//-----------------------------//
// Texture Specific Parameters //
//-----------------------------//

// TODO: DEPTH_STENCIL_TEXTURE_MODE

size_t Texture::getBaseLevel() const
{
  return getParameter<GL_TEXTURE_BASE_LEVEL, GLuint>(getUnitManager().getTargetBinding(m_id));
}

size_t Texture::getMaxLevel() const
{
  return getParameter<GL_TEXTURE_MAX_LEVEL, GLuint>(getUnitManager().getTargetBinding(m_id));
}

// TODO: TEXTURE_SWIZZLE

// TODO: DEPTH_STENCIL_TEXTURE_MODE

void Texture::setBaseLevel(size_t value)
{
  setParameter<GL_TEXTURE_BASE_LEVEL, GLuint>(getUnitManager().getTargetBinding(m_id), value);
}

void Texture::setMaxLevel(size_t value)
{
  setParameter<GL_TEXTURE_MAX_LEVEL, GLuint>(getUnitManager().getTargetBinding(m_id), value);
}

// TODO: TEXTURE_SWIZZLE

//---------------------------//
// Sampler Linked Parameters //
//---------------------------//

GLenum Texture::getMagFilter() const
{
  return getParameter<GL_TEXTURE_MAG_FILTER, GLenum>(getUnitManager().getTargetBinding(m_id));
}

GLenum Texture::getMinFilter() const
{
  return getParameter<GL_TEXTURE_MIN_FILTER, GLenum>(getUnitManager().getTargetBinding(m_id));
}

size_t Texture::getMinLOD() const
{
  return getParameter<GL_TEXTURE_MIN_LOD, GLuint>(getUnitManager().getTargetBinding(m_id));
}

size_t Texture::getMaxLOD() const
{
  return getParameter<GL_TEXTURE_MAX_LOD, GLuint>(getUnitManager().getTargetBinding(m_id));
}

// TODO:TEXTURE_WRAP, Vector or scalar ?

Vector4 Texture::getBorderColor() const {
  return getParameterv<GL_TEXTURE_BORDER_COLOR, Vector4>(getUnitManager().getTargetBinding(m_id));
}

// TODO: TEXTURE_COMPARE_MODE

GLenum Texture::getCompareFunction() const
{
  return getParameter<GL_TEXTURE_COMPARE_FUNC, GLenum>(getUnitManager().getTargetBinding(m_id));
}

void Texture::setMagFilter(GLenum value)
{
  setParameter<GL_TEXTURE_MAG_FILTER, GLenum>(getUnitManager().getTargetBinding(m_id), value);
}

void Texture::setMinFilter(GLenum value)
{
  setParameter<GL_TEXTURE_MIN_FILTER, GLenum>(getUnitManager().getTargetBinding(m_id), value);
}

void Texture::setMinLOD(size_t value)
{
  setParameter<GL_TEXTURE_MIN_LOD, GLuint>(getUnitManager().getTargetBinding(m_id), value);
}

void Texture::setMaxLOD(size_t value)
{
  setParameter<GL_TEXTURE_MAX_LOD, GLuint>(getUnitManager().getTargetBinding(m_id), value);
}

// TODO:TEXTURE_WRAP, Vector or scalar ?

void Texture::setBorderColor(const Vector4 &value)
{
  setParameterv<GL_TEXTURE_BORDER_COLOR, Vector4>(getUnitManager().getTargetBinding(m_id), value);
}

// TODO: TEXTURE_COMPARE_MODE

void Texture::setCompareFunction(GLenum value)
{
  setParameter<GL_TEXTURE_COMPARE_FUNC, GLenum>(getUnitManager().getTargetBinding(m_id), value);
}

//==========================//
// Texture Level Parameters //
//==========================//

namespace
{
  inline void _getLevelParameter(
    GLenum target,
    size_t level,
    GLenum parameter,
    GLfloat *value
  )
  {
    glGetTexLevelParameterfv(
      target,
      level,
      parameter,
      value
    );
  }

  inline void _getLevelParameter(
    GLenum target,
    size_t level,
    GLenum parameter,
    GLint *value
  )
  {
    glGetTexLevelParameteriv(
      target,
      level,
      parameter,
      value
    );
  }

  inline void _getLevelParameter(
    GLenum target,
    size_t level,
    GLenum parameter,
    GLuint *value
  )
  {
    GLint uvalue;
    glGetTexLevelParameteriv(
      target,
      level,
      parameter,
      &uvalue
    );
    *value = uvalue;
  }

  template <GLenum PARAMETER, typename T>
  inline static T getLevelParameter(GLenum target, size_t level)
  {
    T value;
    _getLevelParameter(target, level, PARAMETER, &value);
    return value;
  }
}

size_t Texture::getWidth(size_t level) const
{
  return getLevelParameter<GL_TEXTURE_WIDTH, GLuint>(getUnitManager().getTargetBinding(m_id), level);
}

size_t Texture::getHeight(size_t level) const
{
  return getLevelParameter<GL_TEXTURE_HEIGHT, GLuint>(getUnitManager().getTargetBinding(m_id), level);
}

size_t Texture::getDepth(size_t level) const
{
  return getLevelParameter<GL_TEXTURE_DEPTH, GLuint>(getUnitManager().getTargetBinding(m_id), level);
}

size_t Texture::getSamples(size_t level) const
{
  return getLevelParameter<GL_TEXTURE_SAMPLES, GLuint>(getUnitManager().getTargetBinding(m_id), level);
}

bool Texture::getFixedSampleLocations(size_t level) const
{
  return getLevelParameter<GL_TEXTURE_FIXED_SAMPLE_LOCATIONS, GLint>(getUnitManager().getTargetBinding(m_id), level);
}

// TODO: GL_TEXTURE_INTERNAL_FORMAT

// TODO: GL_TEXTURE_SHARED_SIZE

bool Texture::getCompressed(size_t level) const
{
  return getLevelParameter<GL_TEXTURE_COMPRESSED, GLint>(getUnitManager().getTargetBinding(m_id), level);
}

size_t Texture::getCompressedImageSize(size_t level) const
{
  return getLevelParameter<GL_TEXTURE_COMPRESSED_IMAGE_SIZE, GLuint>(getUnitManager().getTargetBinding(m_id), level);
}

// TODO: GL_TEXTURE_BUFFER_DATA_STORE_BINDING

int Texture::getBufferOffset(size_t level) const
{
  return getLevelParameter<GL_TEXTURE_BUFFER_OFFSET, GLuint>(getUnitManager().getTargetBinding(m_id), level);
}

size_t Texture::getBufferSize(size_t level) const
{
  return getLevelParameter<GL_TEXTURE_BUFFER_SIZE, GLuint>(getUnitManager().getTargetBinding(m_id), level);
}
