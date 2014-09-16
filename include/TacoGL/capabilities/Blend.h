#ifndef __TACOGL_BLEND__
#define __TACOGL_BLEND__

#include <TacoGL/OpenGL.h>
#include <TacoGL/Error.h>
#include <TacoGL/get.h>

#include <TacoGL/capabilities/Capability.h>

namespace TacoGL
{

class Blend : public Capability<gl::GL_BLEND, true>
{
public:
    // enum class gl::GLenum : GLenum
    // {
    //     ZERO                     = GL_ZERO,
    //     ONE                      = GL_ONE,
    //     SRC_COLOR                = GL_SRC_COLOR,
    //     ONE_MINUS_SRC_COLOR      = GL_ONE_MINUS_SRC_COLOR,
    //     DST_COLOR                = GL_DST_COLOR,
    //     ONE_MINUS_DST_COLOR      = GL_ONE_MINUS_DST_COLOR,
    //     SRC_ALPHA                = GL_SRC_ALPHA,
    //     ONE_MINUS_SRC_ALPHA      = GL_ONE_MINUS_SRC_ALPHA,
    //     DST_ALPHA                = GL_DST_ALPHA,
    //     ONE_MINUS_DST_ALPHA      = GL_ONE_MINUS_DST_ALPHA,
    //     CONSTANT_COLOR           = GL_CONSTANT_COLOR,
    //     ONE_MINUS_CONSTANT_COLOR = GL_ONE_MINUS_CONSTANT_COLOR,
    //     CONSTANT_ALPHA           = GL_CONSTANT_ALPHA,
    //     ONE_MINUS_CONSTANT_ALPHA = GL_ONE_MINUS_CONSTANT_ALPHA,
    //     SRC_ALPHA_SATURATE       = GL_SRC_ALPHA_SATURATE,
    //     SRC1_COLOR               = GL_SRC1_COLOR,
    //     ONE_MINUS_SRC1_COLOR     = GL_ONE_MINUS_SRC1_COLOR,
    //     SRC1_ALPHA               = GL_SRC1_ALPHA,
    //     ONE_MINUS_SRC1_ALPHA     = GL_ONE_MINUS_SRC1_ALPHA,
    // };

    // enum class gl::GLenum : GLenum
    // {
    //     ADD               = GL_FUNC_ADD,
    //     SUBSTRACT         = GL_FUNC_SUBTRACT,
    //     REVERSE_SUBSTRACT = GL_FUNC_REVERSE_SUBTRACT,
    //     MIN               = GL_MIN,
    //     MAX               = GL_MAX
    // };


    // ACCESSORS

    static Vector4 getColor()
    {
        return getv<gl::GL_BLEND_COLOR, Vector4>();
    }

    static gl::GLenum getFunctionSrcColor()
    {
        return static_cast<gl::GLenum>(get<gl::GL_BLEND_SRC_RGB, gl::GLint>());
    }

    static gl::GLenum getFunctionSrcAlpha()
    {
        return static_cast<gl::GLenum>(get<gl::GL_BLEND_SRC_ALPHA, gl::GLint>());
    }

    static gl::GLenum getFunctionDstColor()
    {
        return static_cast<gl::GLenum>(get<gl::GL_BLEND_DST_RGB, gl::GLint>());
    }

    static gl::GLenum getFunctionDstAlpha()
    {
        return static_cast<gl::GLenum>(get<gl::GL_BLEND_DST_ALPHA, gl::GLint>());
    }

    static gl::GLenum getEquationColor()
    {
        return static_cast<gl::GLenum>(get<gl::GL_BLEND_EQUATION_RGB, gl::GLint>());
    }

    static gl::GLenum getEquationAlpha()
    {
        return static_cast<gl::GLenum>(get<gl::GL_BLEND_EQUATION_ALPHA, gl::GLint>());
    }


    // MUTATORS

    static void setFunction(gl::GLenum src, gl::GLenum dst)
    {
        gl::glBlendFunc(src, dst);
    }

    static void setFunctionIndexed(gl::GLuint index, gl::GLenum src, gl::GLenum dst)
    {
        gl::glBlendFunci(index, src, dst);
    }

    static void setFunctionSeparate(
            gl::GLenum srcColor, gl::GLenum dstColor,
            gl::GLenum srcAlpha, gl::GLenum dstAlpha
    )
    {
        gl::glBlendFuncSeparate(
            srcColor, dstColor,
            srcAlpha, dstAlpha
        );
    }

    static void setFunctionSeparateIndexed(
            gl::GLuint index,
            gl::GLenum srcColor, gl::GLenum dstColor,
            gl::GLenum srcAlpha, gl::GLenum dstAlpha
    )
    {
        gl::glBlendFuncSeparatei(
            index,
            srcColor, dstColor,
            srcAlpha, dstAlpha
        );
    }

    static void setEquation(gl::GLenum equation)
    {
        gl::glBlendEquation(equation);
    }

    static void setEquationi(gl::GLuint index, gl::GLenum equation)
    {
        gl::glBlendEquationi(index, equation);
    }
};

} // end namespace GL

#endif
