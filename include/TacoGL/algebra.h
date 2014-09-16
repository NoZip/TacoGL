#ifndef __GL_ALGEBRA__
#define __GL_ALGEBRA__

#include <Morpheus/Vector.h>
#include <Morpheus/Matrix.h>

#include <TacoGL/OpenGL.h>

namespace TacoGL
{

using Vector2 = Morpheus::Vector<gl::GLfloat, 2>;
using Vector2i = Morpheus::Vector<gl::GLint, 2>;
using Vector2ui = Morpheus::Vector<gl::GLuint, 2>;
using Vector2b = Morpheus::Vector<gl::GLboolean, 2>;

using Vector3 = Morpheus::Vector<gl::GLfloat, 3>;
using Vector3i = Morpheus::Vector<gl::GLint, 3>;
using Vector3ui = Morpheus::Vector<gl::GLuint, 3>;
using Vector32b = Morpheus::Vector<gl::GLboolean, 3>;

using Vector4 = Morpheus::Vector<gl::GLfloat, 4>;
using Vector4i = Morpheus::Vector<gl::GLint, 4>;
using Vector4ui = Morpheus::Vector<gl::GLuint, 4>;
using Vector4b = Morpheus::Vector<gl::GLboolean, 4>;


using Matrix2 = Morpheus::Matrix<gl::GLfloat, 2>;
using Matrix3 = Morpheus::Matrix<gl::GLfloat, 3>;
using Matrix4 = Morpheus::Matrix<gl::GLfloat, 4>;

} // end namespace GL

#endif
