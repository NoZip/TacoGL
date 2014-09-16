#ifndef __TACOGL_CAPABILITY__
#define __TACOGL_CAPABILITY__

#include <cassert>

#include <TacoGL/OpenGL.h>
#include <TacoGL/Error.h>

namespace TacoGL
{

/**
 * User Friendly class, used to interface OpenGL capabilities.
 * @tparam CAPABILITY The capability to interface.
 * @tparam INDEXED If the capability is indexed.
 */
template <gl::GLenum CAPABILITY, bool INDEXED = false>
class Capability
{
public:
    /**
     * Enable capability.
     */
    static void enable()
    {
        gl::glEnable(CAPABILITY);
    }

    /**
     * Disable capability.
     */
    static void disable()
    {
        gl::glDisable(CAPABILITY);
    }

    /**
     * Check if capability is enabled.
     * @return Capability.
     */
    static bool isEnabled()
    {
        gl::GLboolean value = gl::glIsEnabled(CAPABILITY);

        return (value == gl::GL_TRUE) ? true : false;
    }

    /**
     * Enable capability (indexed version).
     * @param index The capability index to enable.
     */
    static void enable(gl::GLuint index)
    {
        assert(INDEXED);

        gl::glEnablei(CAPABILITY, index);
    }

    /**
     * Disable capability (indexed version).
     * @param index The capability index to disable.
     */
    static void disable(gl::GLuint index)
    {
        assert(INDEXED);

        gl::glDisablei(CAPABILITY, index);
    }

    /**
     * Check if capability is enabled (indexed version).
     * @param index The capability index to check.
     * @return Capability.
     */
    static bool isEnabled(gl::GLuint index)
    {
        assert(INDEXED);

        gl::GLboolean value = gl::glIsEnabledi(CAPABILITY, index);

        return (value == gl::GL_TRUE) ? true : false;
    }
};

} // end namespace GL

#endif
