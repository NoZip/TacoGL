#include <cstdlib>
#include <cassert>
#include <iostream>

#include <glbinding/Meta.h>

#include <TacoGL/Error.h>

#include <TacoGL/Buffer.h>

using namespace gl;
using namespace glbinding;
using namespace TacoGL;

Buffer::Manager::Manager()
{

}

Buffer::Manager::~Manager()
{

}

const typename Buffer::Manager::BindedTargetMap &
Buffer::Manager::getTarget() const
{
  return m_target;
}

const typename Buffer::Manager::BindingMap &
Buffer::Manager::getBinding() const
{
  return m_binding;
}

GLenum Buffer::Manager::getBinding(GLuint bufferId) const
{
  return m_binding.at(bufferId);
}

void Buffer::Manager::bind(GLenum target, GLuint bufferId)
{
  assert(m_target.find(target) == m_target.end());

  glBindBuffer(target, bufferId);

  m_target.insert(target);
  m_binding.emplace(bufferId, target);
}

void Buffer::Manager::unbind(GLuint bufferId)
{
  GLenum target = m_binding.at(bufferId);

  assert(m_target.find(target) != m_target.end());

  glBindBuffer(target, 0);

  m_target.erase(target);
  m_binding.erase(bufferId);
}

void Buffer::Manager::debug() const
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

Buffer::Manager Buffer::s_manager;

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

void Buffer::bind(gl::GLenum target)
{
  s_manager.bind(target, m_id);
}

void Buffer::unbind()
{
  s_manager.unbind(m_id);
}
