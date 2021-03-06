#ifndef __TacoGL_ATTRIBUTE_FORMAT__
#define __TacoGL_ATTRIBUTE_FORMAT__

#include <unordered_map>

#include <TacoGL/OpenGL.h>
#include <TacoGL/Error.h>
#include <TacoGL/Program.h>
#include <TacoGL/Buffer.h>

namespace TacoGL
{

  class AbstractAttributeFormat
  {
  public:
    AbstractAttributeFormat(size_t size, gl::GLenum type, size_t offset)
    : m_size(size), m_type(type), m_offset(offset)
    {

    }

    virtual ~AbstractAttributeFormat() = default;

    size_t getSize() const { return m_size; }
    gl::GLenum getType() const { return m_type; }
    size_t getOffset() const { return m_offset; }

    virtual void init(gl::GLint location, size_t stride) = 0;

  protected:
    size_t m_size;
    gl::GLenum m_type;
    size_t m_offset;
  };

  template <gl::GLenum Type, typename T>
  class AttributeFormat;

  namespace
  {
    template <typename T>
    inline gl::GLenum _type();

    template <>
    inline gl::GLenum _type<gl::GLfloat>()
    {
      return gl::GL_FLOAT;
    }

    template <>
    inline gl::GLenum _type<gl::GLbyte>()
    {
      return gl::GL_BYTE;
    }

    template <>
    inline gl::GLenum _type<gl::GLubyte>()
    {
      return gl::GL_UNSIGNED_BYTE;
    }

    template <>
    inline gl::GLenum _type<gl::GLshort>()
    {
      return gl::GL_SHORT;
    }

    template <>
    inline gl::GLenum _type<gl::GLushort>()
    {
      return gl::GL_UNSIGNED_SHORT;
    }

    template <>
    inline gl::GLenum _type<gl::GLint>()
    {
      return gl::GL_INT;
    }

    template <>
    inline gl::GLenum _type<gl::GLuint>()
    {
      return gl::GL_UNSIGNED_INT;
    }
  }

  template <typename T>
  class AttributeFormat<gl::GL_FLOAT, T> : public AbstractAttributeFormat
  {
  public:
    AttributeFormat(
      size_t offset,
      bool normalized = false
    )
    : AbstractAttributeFormat(T::RowsAtCompileTime, _type<typename T::Scalar>(), offset), m_normalized(normalized)
    {

    }

    virtual ~AttributeFormat() = default;

    bool isNormalized() const { return m_normalized; }

    virtual void init(gl::GLint location, size_t stride)
    {
      gl::glVertexAttribPointer(
        location,
        m_size,
        m_type,
        (m_normalized) ? gl::GL_TRUE : gl::GL_FALSE,
        stride,
        (const void*) m_offset
      );
    }

  protected:
    bool m_normalized;
  };

class VertexBufferFormat
{
public:
    using AttributeMap = std::unordered_map<std::string, AbstractAttributeFormat*>;

    VertexBufferFormat(size_t stride);

    virtual ~VertexBufferFormat();

    const AttributeMap & getAttributes() const { return m_attributes; }

    template <gl::GLenum Type, typename T>
    void add(const std::string &name, const AttributeFormat<Type, T> &format)
    {
      m_attributes.emplace(name, new AttributeFormat<Type, T>(format));
    }

    void init(const Program &program, Buffer &buffer);

protected:
    AttributeMap m_attributes;
    size_t m_stride;
};

} // end GL namepspace

#endif
