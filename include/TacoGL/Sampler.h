#ifndef __TACOGL_SAMPLER__
#define __TACOGL_SAMPLER__

#include <TacoGL/OpenGL.h>
#include <TacoGL/Error.h>
#include <TacoGL/algebra.h>
#include <TacoGL/Object.h>


namespace TacoGL
{

  class Sampler : public Object
  {
  public:
    Sampler();
    virtual ~Sampler();

    void bind(size_t unit);

    gl::GLenum getMagFilter() const;
    gl::GLenum getMinFilter() const;
    size_t getMinLOD() const;
    size_t getMaxLOD() const;
    // TODO:TEXTURE_WRAP, Vector or scalar ?
    Vector4 getBorderColor() const;
    // TODO: TEXTURE_COMPARE_MODE
    gl::GLenum getCompareFunction() const;

    void setMagFilter(gl::GLenum value);
    void setMinFilter(gl::GLenum value);
    void setMinLOD(size_t value);
    void setMaxLOD(size_t value);
    // TODO:TEXTURE_WRAP, Vector or scalar ?
    void setBorderColor(const Vector4 &value);
    // TODO: TEXTURE_COMPARE_MODE
    void setCompareFunction(gl::GLenum value);
  };

} // end namespace GL

#endif