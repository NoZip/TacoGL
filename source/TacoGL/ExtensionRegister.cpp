#include <cassert>

#include <glbinding/Meta.h>

#include <TacoGL/ExtensionRegister.h>

using namespace glbinding;
using namespace gl;
using namespace TacoGL;

std::unordered_set<gl::GLextension> ExtensionRegister::s_extensions;

std::unordered_set<gl::GLextension> & ExtensionRegister::getExtensions()
{
  if (s_extensions.empty())
  {
    for (auto extension : Meta::extensions())
    {
      s_extensions.insert(extension);
    }
  }

  return s_extensions;
}

bool ExtensionRegister::isAvaible(GLextension ext)
{
  auto extensions = getExtensions();
  return (extensions.find(ext) != extensions.end());
}