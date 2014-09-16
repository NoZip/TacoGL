#include <TacoGL/VertexArray.h>

using namespace TacoGL;

VertexArray::VertexArray()
{
    gl::glGenVertexArrays(1, &m_id);
}

VertexArray::~VertexArray()
{
    gl::glDeleteVertexArrays(1, &m_id);
}

void VertexArray::bind()
{
    gl::glBindVertexArray(m_id);
}

void VertexArray::unbind()
{
    gl::glBindVertexArray(0);
}
