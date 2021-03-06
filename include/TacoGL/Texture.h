#ifndef __TACOGL_TEXTURE__
#define __TACOGL_TEXTURE__

#include <vector>
#include <unordered_map>
#include <unordered_set>

#include <TacoGL/OpenGL.h>
#include <TacoGL/Error.h>
#include <TacoGL/algebra.h>
#include <TacoGL/Object.h>
#include <TacoGL/Sampler.h>

namespace TacoGL
{
  /**
   * Manages OpenGL texture bindings.
   */
  class TextureUnitManager
  {
  public:
    using UnitUsageSet = std::unordered_set<size_t>;
    using BindingPair = std::pair<size_t, gl::GLenum>;
    using BindingMap = std::unordered_map<gl::GLuint, BindingPair>;

    TextureUnitManager() = default;
    virtual ~TextureUnitManager() = default;

    bool isAvaible(size_t unit) const;
    bool isBinded(gl::GLuint textureId) const;
    size_t getUnitBinding(gl::GLuint textureId) const;
    gl::GLenum getTargetBinding(gl::GLuint textureId) const;

    /**
     * Bind a Texture to an OpenGL unit texture.
     * @param texture The texture to bind.
     * @param unit    The unit where to bind the Texture.
     */
    void bind(size_t unit, gl::GLenum target, gl::GLuint textureId, gl::GLuint samplerId = 0);

    /**
     * Bind a texture to the first avaible unit texture.
     * @param texture the texture to bind.
     */
    size_t bind(gl::GLenum target, gl::GLuint textureId, gl::GLuint samplerId = 0);

    /**
     * Unbind a texture.
     * @param texture The texture to unbind.
     */
    void unbind(gl::GLuint textureId);

    void unbindAll();

  protected:
    UnitUsageSet m_unitUsage;
    BindingMap m_unitBinding;
  };

  class ImageUnitManager
  {
  public:
    struct ImageBinding
    {
      size_t unit;
      size_t level;
      bool layered;
      size_t layer;
      gl::GLenum access;
      gl::GLenum format;
    };

    using UnitUsageSet = std::unordered_set<size_t>;
    using BindingMap = std::unordered_map<gl::GLuint, ImageBinding>;

    static size_t getImageUnitCount();

    ImageUnitManager() = default;
    virtual ~ImageUnitManager() = default;

    bool isAvaible(size_t unit) const;
    bool isBinded(gl::GLuint textureId) const;
    const ImageBinding& getBinding(gl::GLuint textureId) const;
    size_t getUnitBinding(gl::GLuint textureId) const;

    void bind(
      size_t unit,
      gl::GLuint textureId,
      size_t level,
      bool layered,
      size_t layer,
      gl::GLenum access,
      gl::GLenum format
    );

    void unbind(gl::GLuint textureId);

    void unbindAll();

  protected:
    UnitUsageSet m_unitUsage;
    BindingMap m_binding;
  };

  class Texture : public Object
  {
  public:

    /**
     * Retrieve the number of texture units.
     */
    static size_t getTextureUnitCount();

    /**
     * Retrieve the actual active texture unit.
     */
    static size_t getActiveTextureUnit();

    /**
     * Set the active texture unit.
     * @param unit the unit to make active.
     */
    static void setActiveTextureUnit(size_t unit);

    static const TextureUnitManager& getTextureUnitManager();
    static const ImageUnitManager& getImageUnitManager();

    Texture();
    virtual ~Texture();

    Sampler* getSampler() const { return m_sampler; }
    bool isBinded() const;
    gl::GLenum getTarget() const;

    void setSampler(Sampler *sampler) { m_sampler = sampler; }
    
    /**
     * Bind texture to a texture unit.
     * @param unit the unit to bind the texture.
     */
    void bind(size_t unit, gl::GLenum target);

    /**
     * Bind texture to the first avaible texture unit.
     */
    size_t bind(gl::GLenum target);

    /**
     * Unbind texture.
     */
    void unbind();

    void unbindAll();

    void bindImage(size_t unit, size_t level, size_t layer, gl::GLenum access, gl::GLenum format);

    /**
     * TODO
     */
    void getData(
      size_t level,
      gl::GLenum format,
      gl::GLenum type,
      void *img
    ) const;

    /**
     * Updates texture data, 1 dimension version.
     * @param level          the texture level (or layer).
     * @param format         the data format.
     * @param internalFormat the data internal format.
     * @param type           the data type.
     * @param size           the data size.
     * @param data           the data to set to the texture.
     */
    void setData(
      size_t level,
      gl::GLenum format,
      gl::GLenum internalFormat,
      gl::GLenum type,
      size_t size,
      void *data
    );

    /**
     * [setData description]
     * @param level          [description]
     * @param format         [description]
     * @param internalFormat [description]
     * @param type           [description]
     * @param width          [description]
     * @param height         [description]
     * @param data           [description]
     */
    void setData(
      size_t level,
      gl::GLenum format,
      gl::GLenum internalFormat,
      gl::GLenum type,
      size_t width, size_t height,
      void *data
    );

    /**
     * [setData description]
     * @param level          [description]
     * @param format         [description]
     * @param internalFormat [description]
     * @param type           [description]
     * @param width          [description]
     * @param height         [description]
     * @param depth          [description]
     * @param data           [description]
     */
    void setData(
      size_t level,
      gl::GLenum format,
      gl::GLenum internalFormat,
      gl::GLenum type,
      size_t width, size_t height, size_t depth,
      void *data
    );

    void generateMipmaps();

    //--------------------//
    // Texture Parameters //
    //--------------------//

    // TODO: DEPTH_STENCIL_TEXTURE_MODE
    // gl::GLenum getDepthStencilMode() const;
    size_t getBaseLevel() const;
    size_t getMaxLevel() const;
    gl::GLenum getSwizzleR() const;
    gl::GLenum getSwizzleG() const;
    gl::GLenum getSwizzleB() const;
    gl::GLenum getSwizzleA() const;

    // TODO: DEPTH_STENCIL_TEXTURE_MODE
    // void setDepthStencilMode(gl::GLenum value);
    void setBaseLevel(size_t value);
    void setMaxLevel(size_t value);
    void setSwizzleR(gl::GLenum value);
    void setSwizzleG(gl::GLenum value);
    void setSwizzleB(gl::GLenum value);
    void setSwizzleA(gl::GLenum value);
    // TODO: swizzle RGBA

    //---------------------//
    // Sampling Parameters //
    //---------------------//

    gl::GLenum getMagFilter() const;
    gl::GLenum getMinFilter() const;
    size_t getMinLOD() const;
    size_t getMaxLOD() const;
    gl::GLenum getWrapS() const;
    gl::GLenum getWrapT() const;
    gl::GLenum getWrapR() const;
    Vector4 getBorderColor() const;
    // TODO: TEXTURE_COMPARE_MODE
    gl::GLenum getCompareFunction() const;

    void setMagFilter(gl::GLenum value);
    void setMinFilter(gl::GLenum value);
    void setMinLOD(size_t value);
    void setMaxLOD(size_t value);
    void setWrapS(gl::GLenum value);
    void setWrapT(gl::GLenum value);
    void setWrapR(gl::GLenum value);
    void setWrap(gl::GLenum s);
    void setWrap(gl::GLenum s, gl::GLenum t);
    void setWrap(gl::GLenum s, gl::GLenum t, gl::GLenum r);
    void setBorderColor(const Vector4 &value);
    // TODO: TEXTURE_COMPARE_MODE
    void setCompareFunction(gl::GLenum value);

    //-------------------------//
    // Texture Level Parameter //
    //-------------------------//
    
    size_t getWidth(size_t level = 0) const;
    size_t getHeight(size_t level = 0) const;
    size_t getDepth(size_t level = 0) const;
    size_t getSamples(size_t level = 0) const;
    bool getFixedSampleLocations(size_t level = 0) const;
    gl::GLenum getInternalFormat(size_t level = 0) const;
    // TODO: GL_TEXTURE_SHARED_SIZE
    bool getCompressed(size_t level = 0) const;
    size_t getCompressedImageSize(size_t level = 0) const;
    // TODO: GL_TEXTURE_BUFFER_DATA_STORE_BINDING
    int getBufferOffset(size_t level = 0) const;
    size_t getBufferSize(size_t level = 0) const;

  protected:
    static TextureUnitManager s_textureUnitManager;
    static ImageUnitManager s_imageUnitManager;

    Sampler *m_sampler;
  };
  
} // end namespace GL

#endif
