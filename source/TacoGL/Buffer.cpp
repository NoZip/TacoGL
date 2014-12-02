#include <cstdlib>
#include <cassert>
#include <iostream>

#include <glbinding/Meta.h>

#include <TacoGL/Error.h>

#include <TacoGL/Buffer.h>

using namespace gl;
using namespace glbinding;
using namespace TacoGL;

BufferManager::BufferManager()
{

}

BufferManager::~BufferManager()
{

}

const BufferManager::BindedTargetMap &
BufferManager::getTarget() const
{
  return m_target;
}

const BufferManager::BindingMap &
BufferManager::getBinding() const
{
  return m_binding;
}

bool BufferManager::isAvaible(gl::GLenum target) const
{
  return m_target.find(target) == m_target.end();
}

bool BufferManager::isBinded(GLuint bufferId) const
{
  return m_binding.find(bufferId) != m_binding.end();
}

// bool isBinded(gl::GLuint bufferId, gl::GLenum target) const
// {
//   if (!isBinded(bufferId))
//     return false;

//   return m_binding.at(bufferId) == target;
// }

GLenum BufferManager::getBinding(GLuint bufferId) const
{
  // assert(isBinded(bufferId));
  return m_binding.at(bufferId);
}

void BufferManager::bind(GLenum target, GLuint bufferId)
{
  // assert(isAvaible(target));

  glBindBuffer(target, bufferId);

  m_target.insert(target);
  m_binding.emplace(bufferId, target);
}

void BufferManager::unbind(GLuint bufferId)
{
  // assert(isBinded(bufferId));

  GLenum target = m_binding.at(bufferId);

  glBindBuffer(target, 0);

  m_target.erase(target);
  m_binding.erase(bufferId);
}

void BufferManager::debug() const
{
  std::cout << "DEBUG: buffer target map" << std::endl;

  for (auto &target : m_target) {
    std::cout << "Target " << Meta::getString(target) << std::endl;
  }

  std::cout << "DEBUG: buffer binding map" << std::endl;

  for (auto &item : m_binding) {
    std::cout << "Buffer " << item.first << " binded to " << Meta::getString(item.second) << std::endl;
  }
}

//=
// Target Binging Getters //
//=

template <>
gl::GLuint Buffer::getBinding<gl::GL_ARRAY_BUFFER>()
{
  return TacoGL::get<gl::GL_ARRAY_BUFFER_BINDING, gl::GLuint>();
}

template <>
gl::GLuint Buffer::getBinding<gl::GL_ATOMIC_COUNTER_BUFFER>()
{
  return TacoGL::get<gl::GL_ATOMIC_COUNTER_BUFFER_BINDING, gl::GLuint>();
}

template <>
gl::GLuint Buffer::getBinding<gl::GL_COPY_READ_BUFFER>()
{
  return TacoGL::get<gl::GL_COPY_READ_BUFFER_BINDING, gl::GLuint>();
}

template <>
gl::GLuint Buffer::getBinding<gl::GL_COPY_WRITE_BUFFER>()
{
  return TacoGL::get<gl::GL_COPY_WRITE_BUFFER_BINDING, gl::GLuint>();
}

template <>
gl::GLuint Buffer::getBinding<gl::GL_DRAW_INDIRECT_BUFFER>()
{
  return TacoGL::get<gl::GL_DRAW_INDIRECT_BUFFER_BINDING, gl::GLuint>();
}

template <>
gl::GLuint Buffer::getBinding<gl::GL_DISPATCH_INDIRECT_BUFFER>()
{
  return TacoGL::get<gl::GL_DISPATCH_INDIRECT_BUFFER_BINDING, gl::GLuint>();
}

template <>
gl::GLuint Buffer::getBinding<gl::GL_ELEMENT_ARRAY_BUFFER>()
{
  return TacoGL::get<gl::GL_ELEMENT_ARRAY_BUFFER_BINDING, gl::GLuint>();
}

template <>
gl::GLuint Buffer::getBinding<gl::GL_PIXEL_PACK_BUFFER>()
{
  return TacoGL::get<gl::GL_PIXEL_PACK_BUFFER_BINDING, gl::GLuint>();
}

template <>
gl::GLuint Buffer::getBinding<gl::GL_PIXEL_UNPACK_BUFFER>()
{
  return TacoGL::get<gl::GL_PIXEL_UNPACK_BUFFER_BINDING, gl::GLuint>();
}

template <>
gl::GLuint Buffer::getBinding<gl::GL_SHADER_STORAGE_BUFFER>()
{
  return TacoGL::get<gl::GL_SHADER_STORAGE_BUFFER_BINDING, gl::GLuint>();
}

template <>
gl::GLuint Buffer::getBinding<gl::GL_TRANSFORM_FEEDBACK_BUFFER>()
{
  return TacoGL::get<gl::GL_TRANSFORM_FEEDBACK_BUFFER_BINDING, gl::GLuint>();
}

template <>
gl::GLuint Buffer::getBinding<gl::GL_UNIFORM_BUFFER>()
{
  return TacoGL::get<gl::GL_UNIFORM_BUFFER_BINDING, gl::GLuint>();
}

BufferManager Buffer::s_manager;

Buffer::Buffer()
: m_size(0)
{
  glGenBuffers(1, &m_id);
}

Buffer::~Buffer()
{
  glDeleteBuffers(1, &m_id);
}

size_t Buffer::getSize() const
{
  return m_size;
}

bool Buffer::isBinded() const
{
  return s_manager.isBinded(m_id);
}

gl::GLenum Buffer::getTarget() const
{
  // assert(isBinded());
  return s_manager.getBinding(m_id);
}

void Buffer::bind(gl::GLenum target)
{
  s_manager.bind(target, m_id);
}

void Buffer::unbind()
{
  s_manager.unbind(m_id);
}
