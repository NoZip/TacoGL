#ifndef __TacoGL_COLOR_LOGIC_OPERATOR__
#define __TacoGL_COLOR_LOGIC_OPERATOR__

#include <TacoGL/OpenGL.h>
#include <TacoGL/Error.h>
#include <TacoGL/get.h>

#include <TacoGL/capabilities/Capability.h>

namespace TacoGL
{

  class ColorLogicOperator : public Capability<gl::GL_COLOR_LOGIC_OP>
  {
  public:
    // enum class Function : GLenum
    // {
    //     CLEAR         = GL_CLEAR,         ///< 0
    //     SET           = GL_SET,           ///< 1
    //     COPY          = GL_COPY,          ///< src
    //     COPY_INVERTED = GL_COPY_INVERTED, ///< ~src
    //     NOOP          = GL_NOOP,          ///< d
    //     INVERT        = GL_INVERT,        ///< ~d
    //     AND           = GL_AND,           ///< src & dst
    //     NAND          = GL_NAND,          ///< ~(src & dst)
    //     OR            = GL_OR,            ///< src | dst
    //     NOR           = GL_NOR,           ///< ~(src | dst)
    //     XOR           = GL_XOR,           ///< src ^ dst
    //     EQUIV         = GL_EQUIV,         ///< ~(src ^ dst)
    //     AND_REVERSE   = GL_AND_REVERSE,   ///< src & ~dst
    //     AND_INVERTED  = GL_AND_INVERTED,  ///< ~src & dst
    //     OR_REVERSE    = GL_OR_REVERSE,    ///< src | ~dst
    //     OR_INVERTED   = GL_OR_INVERTED    ///< ~src | dst
    // };

    static void setFunction(gl::GLenum function)
    {
      gl::glLogicOp(function);
    }
  };

} // end namespace GL

#endif
