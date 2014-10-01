#ifndef __TACOGL_EXTENSION_REGISTER__
#define __TACOGL_EXTENSION_REGISTER__

#include <unordered_set>

#include <TacoGL/OpenGL.h>
#include <TacoGL/Error.h>

class ExtensionRegister
{
public:
  static std::unordered_set<gl::GLextension> & getExtensions();
  static bool isAvaible(gl::GLextension ext);

protected:
  static std::unordered_set<gl::GLextension> s_extensions;
};

#endif
