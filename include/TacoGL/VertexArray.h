#ifndef __TACOGL_VERTEX_ARRAY__
#define __TACOGL_VERTEX_ARRAY__

#include <TacoGL/OpenGL.h>
#include <TacoGL/Error.h>
#include <TacoGL/Object.h>
#include <TacoGL/AttributeFormat.h>

namespace TacoGL
{

  class VertexArray : public Object
  {
  public:
    VertexArray();
    virtual ~VertexArray();

    void bind();
    void unbind();
  };

} // end namespace GL

#endif
