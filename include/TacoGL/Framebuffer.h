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

  class Framebuffer : public Object
  {
  public:
    class Manager
    {
    public:
      Manager();
      ~Manager();

      bool isAvaible(gl::GLenum target) const;
      bool isBinded(gl::GLuint framebufferId) const;
      gl::GLenum getTarget(gl::GLuint framebufferId) const;

      void bind(gl::GLenum target, gl::GLuint framebufferId);
      void unbind(gl::GLuint framebufferId);

    protected:
      std::unordered_set<gl::GLenum> m_target;
      std::unordered_map<gl::GLuint, gl::GLenum> m_binding;
    };

    Framebuffer();
    ~Framebuffer();

    bool isBinded() const;
    gl::GLenum getTarget() const;

    void bind(gl::GLenum target);
    void unbind();

    void attachTexture(
      gl::GLenum attachment​,
      const Texture &texture,
      size_t level
    );

    void attachTextureLayer(
      gl::GLenum attachment​,
      const Texture &texture,
      size_t level,
      size_t layer
    );

    void attachTexture2D(
      gl::GLenum attachment​,
      gl::GLenum textarget​,
      const Texture &texture​,
      size_t level​
    );

    void attachRenderbuffer(
      gl::GLenum attachment,
      const Renderbuffer &renderbuffer
    );

  protected:
    static Manager s_manager;
  };

} // namecpace TacoGL

#endif
