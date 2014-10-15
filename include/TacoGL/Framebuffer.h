#ifndef __TACOGL_FRAMEBUFFER__
#define __TACOGL_FRAMEBUFFER__

#include <unordered_set>
#include <unordered_map>

#include <TacoGL/OpenGL.h>
#include <TacoGL/Error.h>
#include <TacoGL/Object.h>
#include <TacoGL/Texture.h>
#include <TacoGL/Renderbuffer.h>

namespace TacoGL
{
  class FramebufferManager
  {
  public:
    FramebufferManager();
    virtual ~FramebufferManager();

    bool isAvaible(gl::GLenum target) const;
    bool isBinded(gl::GLuint framebufferId) const;
    gl::GLenum getTarget(gl::GLuint framebufferId) const;

    void bind(gl::GLenum target, gl::GLuint framebufferId);
    void unbind(gl::GLuint framebufferId);

  protected:
    std::unordered_set<gl::GLenum> m_target;
    std::unordered_map<gl::GLuint, gl::GLenum> m_binding;
  };

  class Framebuffer : public Object
  {
  public:
    static size_t getMaxDrawBuffers();

    Framebuffer();
    virtual ~Framebuffer();

    bool isBinded() const;
    gl::GLenum getTarget() const;

    void bind(gl::GLenum target);
    void unbind();

    /**
     * Set the number of color attachment in the Framebuffer.
     * @param count the number of color attachment.
     */
    void setDrawBuffers(size_t count);

    void attachTexture(
      size_t attachment​,
      const Texture &texture,
      size_t level
    );

    void attachTexture(
      size_t attachment​,
      gl::GLenum textarget​,
      const Texture &texture​,
      size_t level​
    );

    void attachTextureLayer(
      size_t attachment​,
      const Texture &texture,
      size_t level,
      size_t layer
    );

    void attachRenderbuffer(
      size_t attachment,
      const Renderbuffer &renderbuffer
    );

  protected:
    static FramebufferManager s_manager;
  };

} // namespace TacoGL

#endif
