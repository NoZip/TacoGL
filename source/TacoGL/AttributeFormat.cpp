#include <TacoGL/AttributeFormat.h>

using namespace gl;
using namespace TacoGL;

VertexBufferFormat::VertexBufferFormat(size_t stride)
: m_attributes(), m_stride(stride)
{

}

VertexBufferFormat::~VertexBufferFormat()
{
  for (auto &item : m_attributes)
  {
    delete item.second;
  }
}

// extern template VertexBufferFormat::add<GL_FLOAT>(const std::string &name, const AttributeFormat<GL_FLOAT> &format);

void VertexBufferFormat::init(const Program &program, Buffer &buffer)
{
  if (!buffer.isBinded())
    buffer.bind(GL_ARRAY_BUFFER);

  for (auto &item : m_attributes)
  {
    std::string name = item.first;
    AbstractAttributeFormat *format = item.second;

    GLint location = program.getAttributeLocation(name);

    if (location < 0)
      continue;

    glEnableVertexAttribArray(location);

    format->init(location, m_stride);
  }

  buffer.unbind();
}
