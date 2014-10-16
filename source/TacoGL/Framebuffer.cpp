#include <cassert>

#include <vector>

#include <TacoGL/get.h>
#include <TacoGL/Framebuffer.h>

using namespace gl;
using namespace TacoGL;

//=========//
// Manager //
//=========//

FramebufferManager::FramebufferManager()
{

}

FramebufferManager::~FramebufferManager()
{

}

bool FramebufferManager::isAvaible(GLenum target) const
{
  return m_target.find(target) == m_target.end();
}

bool FramebufferManager::isBinded(GLuint framebufferId) const
{
  return m_binding.find(framebufferId) != m_binding.end();
}

GLenum FramebufferManager::getTarget(GLuint framebufferId) const
{
  assert(isBinded(framebufferId));
  return m_binding.at(framebufferId);
}

void FramebufferManager::bind(GLenum target, GLuint framebufferId)
{
  assert(isAvaible(target));

  glBindFramebuffer(target, framebufferId);

  m_target.insert(target);
  m_binding.emplace(framebufferId, target);
}

void FramebufferManager::unbind(GLuint framebufferId)
{
  assert(isBinded(framebufferId));

  GLenum target = getTarget(framebufferId);

  glBindFramebuffer(target, 0);

  m_target.erase(target);
  m_binding.erase(framebufferId);
}

//=============//
// Framebuffer //
//=============//

size_t Framebuffer::getMaxDrawBuffers()
{
  return get<GL_MAX_DRAW_BUFFERS, size_t>();
}

Framebuffer::Framebuffer()
{
  glGenFramebuffers(1, &m_id);
}

Framebuffer::~Framebuffer()
{
  glDeleteFramebuffers(1, &m_id);
}

bool Framebuffer::isBinded() const
{
  return s_manager.isBinded(m_id);
}

gl::GLenum Framebuffer::getTarget() const
{
  return s_manager.getTarget(m_id);
}

void Framebuffer::bind(GLenum target)
{
  s_manager.bind(target, m_id);
}

void Framebuffer::unbind()
{
  s_manager.unbind(m_id);
}

void Framebuffer::setDrawBuffers(size_t count)
{
  assert(isBinded());

  std::vector<GLenum> buffers(count);

  for (int i = 0; i < count; ++i)
  {
    buffers[i] = static_cast<GLenum>(static_cast<GLuint>(GL_COLOR_ATTACHMENT0) + i);
  }

  glDrawBuffers(count, buffers.data());
}

void Framebuffer::attachTexture(
  gl::GLenum attachment,
  const Texture &texture,
  size_t level
)
{
  assert(isBinded());

  glFramebufferTexture(
    getTarget(),
    attachment,
    texture.getId(),
    level
  );
}

void Framebuffer::attachTexture(
  gl::GLenum attachment,
  GLenum textarget,
  const Texture &texture,
  size_t level
)
{
  assert(isBinded());

  glFramebufferTexture2D(
    getTarget(),
    attachment,
    textarget,
    texture.getId(),
    level
  );
}

void Framebuffer::attachTextureLayer(
  gl::GLenum attachment,
  const Texture &texture,
  size_t level,
  size_t layer
)
{
  assert(isBinded());

  glFramebufferTextureLayer(
    getTarget(),
    attachment,
    texture.getId(),
    level,
    layer
  );
}

void Framebuffer::attachRenderbuffer(
  gl::GLenum attachment,
  const Renderbuffer &renderbuffer
)
{
  assert(isBinded());

  glFramebufferRenderbuffer(
    getTarget(),
    attachment,
    GL_RENDERBUFFER,
    renderbuffer.getId()
  );
}
