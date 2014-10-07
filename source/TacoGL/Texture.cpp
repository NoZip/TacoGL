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

bool TextureUnitManager::isAvaible(size_t unit) const
{
  return (m_unitUsage.find(unit) == m_unitUsage.end());
}

bool TextureUnitManager::isBinded(gl::GLuint textureId) const
{
  return (m_unitBinding.find(textureId) != m_unitBinding.end());
}

size_t TextureUnitManager::getUnitBinding(gl::GLuint textureId) const
{
  return m_unitBinding.at(textureId).first;
}

gl::GLenum TextureUnitManager::getTargetBinding(gl::GLuint textureId) const
{
  return m_unitBinding.at(textureId).second;
}

void TextureUnitManager::bind(size_t unit, GLenum target, GLuint textureId, GLuint samplerId)
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

  m_unitUsage.insert(unit);
  m_unitBinding.emplace(textureId, BindingPair{unit, target});
}

// size_t TextureUnitManager::bind(GLenum target, GLuint textureId, GLuint samplerId)
// {
//   auto firstAvaible = std::find(m_unitUsage.begin(), m_unitUsage.end(), true);

//   size_t unit = firstAvaible - m_unitUsage.begin();

//   bind(unit, target, textureId, samplerId);

//   return unit;
// }

void TextureUnitManager::unbind(GLuint textureId)
{
  assert(isBinded(textureId));

  size_t unit = getUnitBinding(textureId);
  GLenum target = getTargetBinding(textureId);

  m_unitUsage.erase(unit);
  m_unitBinding.erase(textureId);
}

void TextureUnitManager::unbindAll()
{
  m_unitUsage.clear();
  m_unitBinding.clear();
}

//====================//
// Image Unit Manager //
//====================//

bool ImageUnitManager::isAvaible(size_t unit) const
{
  return (m_unitUsage.find(unit) == m_unitUsage.end());
}

bool ImageUnitManager::isBinded(GLuint textureId) const
{
  return (m_binding.find(textureId) != m_binding.end());
}

const typename ImageUnitManager::ImageBinding&
ImageUnitManager::getBinding(GLuint textureId) const
{
  assert(isBinded(textureId));
  return m_binding.at(textureId);
}

size_t ImageUnitManager::getUnitBinding(GLuint textureId) const
{
  return getBinding(textureId).unit;
}

void ImageUnitManager::bind(
  size_t unit,
  GLuint textureId,
  size_t level,
  bool layered,
  size_t layer,
  GLenum access,
  GLenum format
)
{
  assert(isAvaible(unit));
  assert(!isBinded(textureId));

  glBindImageTexture(
    unit,
    textureId,
    level,
    (layered) ? GL_TRUE : GL_FALSE,
    layer,
    access,
    format
  );

  ImageBinding binding = ImageBinding{
    unit,
    level,
    layered,
    layer,
    access,
    format
  };

  m_unitUsage.insert(unit);
  m_binding.emplace(textureId, binding);
}

void ImageUnitManager::unbind(GLuint textureId)
{
  assert(isBinded(textureId));

  size_t unit = getUnitBinding(textureId);

  m_unitUsage.erase(unit);
  m_binding.erase(textureId);
}

void ImageUnitManager::unbindAll()
{
  m_unitUsage.clear();
  m_binding.clear();
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

TextureUnitManager Texture::s_textureUnitManager;
ImageUnitManager Texture::s_imageUnitManager;

const TextureUnitManager& Texture::getTextureUnitManager()
{
  return s_textureUnitManager;
}

const ImageUnitManager& Texture::getImageUnitManager()
{
  return s_imageUnitManager;
}

Texture::Texture() : m_sampler(nullptr)
{
  glGenTextures(1, &m_id);
}

Texture::~Texture()
{
  glDeleteTextures(1, &m_id);
}

bool Texture::isBinded() const
{
  return s_textureUnitManager.isBinded(m_id);
}

GLenum Texture::getTarget() const
{
  assert(isBinded());
  return s_textureUnitManager.getTargetBinding(m_id);
}

//-----------------//
// Texture Binding //
//-----------------//

void Texture::bind(size_t unit, GLenum target)
{
  s_textureUnitManager.bind(unit, target, m_id, (m_sampler) ? m_sampler->getId() : 0);
}

// size_t Texture::bind(GLenum target)
// {
//   return s_textureUnitManager.bind(target, m_id, (m_sampler) ? m_sampler->getId() : 0);
// }

void Texture::unbind()
{
  s_textureUnitManager.unbind(m_id);
}

void Texture::unbindAll()
{
  s_textureUnitManager.unbindAll();
}

void Texture::getData(size_t level, GLenum format, GLenum type, void *img) const
{
  assert(isBinded());
  glGetTexImage(getTarget(), level, format, type, img);
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
  assert(isBinded());
  glTexImage1D(
    getTarget(),
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
  assert(isBinded());
  glTexImage2D(
    getTarget(),
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
  assert(isBinded());
  glTexImage3D(
    getTarget(),
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
  assert(isBinded());
  glGenerateMipmap(getTarget());
}

void Texture::load(const char *filename)
{
  assert(isBinded());

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
  assert(isBinded());
  return getParameter<GL_TEXTURE_BASE_LEVEL, GLuint>(getTarget());
}

size_t Texture::getMaxLevel() const
{
  assert(isBinded());
  return getParameter<GL_TEXTURE_MAX_LEVEL, GLuint>(getTarget());
}

GLenum Texture::getSwizzleR() const
{
  assert(isBinded());
  return getParameter<GL_TEXTURE_SWIZZLE_R, GLenum>(getTarget());
}

GLenum Texture::getSwizzleG() const
{
  assert(isBinded());
  return getParameter<GL_TEXTURE_SWIZZLE_G, GLenum>(getTarget());
}

GLenum Texture::getSwizzleB() const
{
  assert(isBinded());
  return getParameter<GL_TEXTURE_SWIZZLE_B, GLenum>(getTarget());
}

GLenum Texture::getSwizzleA() const
{
  assert(isBinded());
  return getParameter<GL_TEXTURE_SWIZZLE_A, GLenum>(getTarget());
}

// TODO: DEPTH_STENCIL_TEXTURE_MODE

void Texture::setBaseLevel(size_t value)
{
  assert(isBinded());
  setParameter<GL_TEXTURE_BASE_LEVEL, GLuint>(getTarget(), value);
}

void Texture::setMaxLevel(size_t value)
{
  assert(isBinded());
  setParameter<GL_TEXTURE_MAX_LEVEL, GLuint>(getTarget(), value);
}

void Texture::setSwizzleR(GLenum value)
{
  assert(isBinded());
  setParameter<GL_TEXTURE_SWIZZLE_R, GLenum>(getTarget(), value);
}

void Texture::setSwizzleG(GLenum value)
{
  assert(isBinded());
  setParameter<GL_TEXTURE_SWIZZLE_G, GLenum>(getTarget(), value);
}

void Texture::setSwizzleB(GLenum value)
{
  assert(isBinded());
  setParameter<GL_TEXTURE_SWIZZLE_B, GLenum>(getTarget(), value);
}

void Texture::setSwizzleA(GLenum value)
{
  assert(isBinded());
  setParameter<GL_TEXTURE_SWIZZLE_A, GLenum>(getTarget(), value);
}

//---------------------------//
// Sampler Linked Parameters //
//---------------------------//

GLenum Texture::getMagFilter() const
{
  assert(isBinded());
  return getParameter<GL_TEXTURE_MAG_FILTER, GLenum>(getTarget());
}

GLenum Texture::getMinFilter() const
{
  assert(isBinded());
  return getParameter<GL_TEXTURE_MIN_FILTER, GLenum>(getTarget());
}

size_t Texture::getMinLOD() const
{
  assert(isBinded());
  return getParameter<GL_TEXTURE_MIN_LOD, GLuint>(getTarget());
}

size_t Texture::getMaxLOD() const
{
  assert(isBinded());
  return getParameter<GL_TEXTURE_MAX_LOD, GLuint>(getTarget());
}

GLenum Texture::getWrapS() const
{
  assert(isBinded());
  return getParameter<GL_TEXTURE_WRAP_S, GLenum>(getTarget());
}

GLenum Texture::getWrapT() const
{
  assert(isBinded());
  return getParameter<GL_TEXTURE_WRAP_T, GLenum>(getTarget());
}

GLenum Texture::getWrapR() const
{
  assert(isBinded());
  return getParameter<GL_TEXTURE_WRAP_R, GLenum>(getTarget());
}

Vector4 Texture::getBorderColor() const
{
  assert(isBinded());
  return getParameterv<GL_TEXTURE_BORDER_COLOR, Vector4>(getTarget());
}

// TODO: TEXTURE_COMPARE_MODE

GLenum Texture::getCompareFunction() const
{
  assert(isBinded());
  return getParameter<GL_TEXTURE_COMPARE_FUNC, GLenum>(getTarget());
}

void Texture::setMagFilter(GLenum value)
{
  assert(isBinded());
  setParameter<GL_TEXTURE_MAG_FILTER, GLenum>(getTarget(), value);
}

void Texture::setMinFilter(GLenum value)
{
  assert(isBinded());
  setParameter<GL_TEXTURE_MIN_FILTER, GLenum>(getTarget(), value);
}

void Texture::setMinLOD(size_t value)
{
  assert(isBinded());
  setParameter<GL_TEXTURE_MIN_LOD, GLuint>(getTarget(), value);
}

void Texture::setMaxLOD(size_t value)
{
  assert(isBinded());
  setParameter<GL_TEXTURE_MAX_LOD, GLuint>(getTarget(), value);
}

void Texture::setWrapS(GLenum value)
{
  assert(isBinded());
  setParameter<GL_TEXTURE_WRAP_S, GLenum>(getTarget(), value);
}

void Texture::setWrapT(GLenum value)
{
  assert(isBinded());
  setParameter<GL_TEXTURE_WRAP_T, GLenum>(getTarget(), value);
}

void Texture::setWrapR(GLenum value)
{
  assert(isBinded());
  setParameter<GL_TEXTURE_WRAP_R, GLenum>(getTarget(), value);
}

void Texture::setWrap(GLenum s)
{
  assert(isBinded());
  setParameter<GL_TEXTURE_WRAP_S, GLenum>(getTarget(), s);
}

void Texture::setWrap(GLenum s, GLenum t)
{
  assert(isBinded());
  setParameter<GL_TEXTURE_WRAP_S, GLenum>(getTarget(), s);
  setParameter<GL_TEXTURE_WRAP_T, GLenum>(getTarget(), t);
}

void Texture::setWrap(GLenum s, GLenum t, GLenum r)
{
  assert(isBinded());
  setParameter<GL_TEXTURE_WRAP_S, GLenum>(getTarget(), s);
  setParameter<GL_TEXTURE_WRAP_T, GLenum>(getTarget(), t);
  setParameter<GL_TEXTURE_WRAP_R, GLenum>(getTarget(), r);
}

void Texture::setBorderColor(const Vector4 &value)
{
  assert(isBinded());
  setParameterv<GL_TEXTURE_BORDER_COLOR, Vector4>(getTarget(), value);
}

// TODO: TEXTURE_COMPARE_MODE

void Texture::setCompareFunction(GLenum value)
{
  assert(isBinded());
  setParameter<GL_TEXTURE_COMPARE_FUNC, GLenum>(getTarget(), value);
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
    GLenum *value
  )
  {
    GLint uvalue;
    glGetTexLevelParameteriv(
      target,
      level,
      parameter,
      &uvalue
    );
    *value = static_cast<GLenum>(uvalue);
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
  assert(isBinded());
  return getLevelParameter<GL_TEXTURE_WIDTH, GLint>(getTarget(), level);
}

size_t Texture::getHeight(size_t level) const
{
  assert(isBinded());
  return getLevelParameter<GL_TEXTURE_HEIGHT, GLint>(getTarget(), level);
}

size_t Texture::getDepth(size_t level) const
{
  assert(isBinded());
  return getLevelParameter<GL_TEXTURE_DEPTH, GLint>(getTarget(), level);
}

size_t Texture::getSamples(size_t level) const
{
  assert(isBinded());
  return getLevelParameter<GL_TEXTURE_SAMPLES, GLint>(getTarget(), level);
}

bool Texture::getFixedSampleLocations(size_t level) const
{
  assert(isBinded());
  return getLevelParameter<GL_TEXTURE_FIXED_SAMPLE_LOCATIONS, GLint>(getTarget(), level);
}

gl::GLenum Texture::getInternalFormat(size_t level) const
{
  assert(isBinded());
  return getLevelParameter<GL_TEXTURE_INTERNAL_FORMAT, GLenum>(getTarget(), level);
}

// TODO: GL_TEXTURE_SHARED_SIZE

bool Texture::getCompressed(size_t level) const
{
  assert(isBinded());
  return getLevelParameter<GL_TEXTURE_COMPRESSED, GLint>(getTarget(), level);
}

size_t Texture::getCompressedImageSize(size_t level) const
{
  assert(isBinded());
  return getLevelParameter<GL_TEXTURE_COMPRESSED_IMAGE_SIZE, GLint>(getTarget(), level);
}

// TODO: GL_TEXTURE_BUFFER_DATA_STORE_BINDING

int Texture::getBufferOffset(size_t level) const
{
  assert(isBinded());
  return getLevelParameter<GL_TEXTURE_BUFFER_OFFSET, GLint>(getTarget(), level);
}

size_t Texture::getBufferSize(size_t level) const
{
  assert(isBinded());
  return getLevelParameter<GL_TEXTURE_BUFFER_SIZE, GLint>(getTarget(), level);
}
