#ifndef __TACOGL_DRAW__
#define __TACOGL_DRAW__

#include <TacoGL/OpenGL.h>
#include <TacoGL/Error.h>
// #include <TacoGL/type.h>
#include <TacoGL/algebra.h>

namespace TacoGL
{

namespace Renderer
{

inline void viewport(const Vector2i &size, const Vector2i &offset)
{
    gl::glViewport(offset[0], offset[1], size[0], size[1]);
}

inline void setClearColor(const Vector4 &color)
{
    gl::glClearColor(color[0], color[1], color[2], color[3]);
}

inline void clear(gl::ClearBufferMask mask)
{
    gl::glClear(mask);
}

inline void drawArrays(gl::GLenum mode, gl::GLint vertexOffset, size_t count)
{
    gl::glDrawArrays(mode, vertexOffset, count);
}

inline void drawElements(gl::GLenum mode, size_t count, gl::GLenum type, void *indiceOffset)
{
    gl::glDrawElements(
        mode,
        count,
        type,
        indiceOffset
    );
}

struct DrawElementsIndirectCommand
{
    gl::GLuint count;
    gl::GLuint instanceCount;
    gl::GLuint indiceOffset;
    gl::GLuint vertexOffset;
    gl::GLuint baseInstance;
};

inline void drawElementsIndirect(gl::GLenum mode, gl::GLenum type, void *commandOffset)
{
    glDrawElementsIndirect(
        mode,
        type,
        commandOffset
    );
}

}; // end namespace Renderer

}; // end namespace GL

#endif