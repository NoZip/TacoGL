#ifndef __TACOGL_ALGEBRA__
#define __TACOGL_ALGEBRA__

#include <Eigen/Core>
#include <Eigen/Geometry>

#include <TacoGL/OpenGL.h>

namespace TacoGL
{

  namespace
  {
    template <typename T, size_t SIZE>
    using Vector = Eigen::Matrix<T, SIZE, 1>;

    template <typename T, size_t WIDTH, size_t HEIGHT = WIDTH>
    using Matrix = Eigen::Matrix<T, WIDTH, HEIGHT, Eigen::ColMajor>;
  }

  using Vector2 = Vector<gl::GLfloat, 2>;
  using Vector2i = Vector<gl::GLint, 2>;
  using Vector2ui = Vector<gl::GLuint, 2>;
  using Vector2b = Vector<gl::GLboolean, 2>;

  using Vector3 = Vector<gl::GLfloat, 3>;
  using Vector3i = Vector<gl::GLint, 3>;
  using Vector3ui = Vector<gl::GLuint, 3>;
  using Vector3b = Vector<gl::GLboolean, 3>;

  using Vector4 = Vector<gl::GLfloat, 4>;
  using Vector4i = Vector<gl::GLint, 4>;
  using Vector4ui = Vector<gl::GLuint, 4>;
  using Vector4b = Vector<gl::GLboolean, 4>;

  using Matrix2 = Matrix<gl::GLfloat, 2>;
  using Matrix3 = Matrix<gl::GLfloat, 3>;
  using Matrix4 = Matrix<gl::GLfloat, 4>;

  using Matrix2d = Matrix<gl::GLdouble, 2>;
  using Matrix3d = Matrix<gl::GLdouble, 3>;
  using Matrix4d = Matrix<gl::GLdouble, 4>;

  // using Vector2 = Morpheus::Vector<gl::GLfloat, 2>;
  // using Vector2i = Morpheus::Vector<gl::GLint, 2>;
  // using Vector2ui = Morpheus::Vector<gl::GLuint, 2>;
  // using Vector2b = Morpheus::Vector<gl::GLboolean, 2>;

  // using Vector3 = Morpheus::Vector<gl::GLfloat, 3>;
  // using Vector3i = Morpheus::Vector<gl::GLint, 3>;
  // using Vector3ui = Morpheus::Vector<gl::GLuint, 3>;
  // using Vector3b = Morpheus::Vector<gl::GLboolean, 3>;

  // using Vector4 = Morpheus::Vector<gl::GLfloat, 4>;
  // using Vector4i = Morpheus::Vector<gl::GLint, 4>;
  // using Vector4ui = Morpheus::Vector<gl::GLuint, 4>;
  // using Vector4b = Morpheus::Vector<gl::GLboolean, 4>;

  // using Matrix2 = Morpheus::Matrix<gl::GLfloat, 2>;
  // using Matrix3 = Morpheus::Matrix<gl::GLfloat, 3>;
  // using Matrix4 = Morpheus::Matrix<gl::GLfloat, 4>;

  // using Matrix2d = Morpheus::Matrix<gl::GLdouble, 2>;
  // using Matrix3d = Morpheus::Matrix<gl::GLdouble, 3>;
  // using Matrix4d = Morpheus::Matrix<gl::GLdouble, 4>;

} // end namespace TacoGL

#endif
