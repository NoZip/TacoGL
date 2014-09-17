
#include <TacoGL/Renderbuffer.h>

using namespace gl;
using namespace TacoGL;

Renderbuffer::Renderbuffer()
{
  glGenRenderbuffers(1, &m_id);
}

Renderbuffer::~Renderbuffer()
{
  glDeleteRenderbuffers(1, &m_id);
}

void Renderbuffer::bind()
{
  glBindRenderbuffer(GL_RENDERBUFFER, m_id);
}

void Renderbuffer::unbind()
{
  glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void Renderbuffer::allocate(gl::GLenum internalFormat, size_t width, size_t height)
{
  glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height);
}
