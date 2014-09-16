#ifndef __TACOGL_ERROR__
#define __TACOGL_ERROR__

#include <exception>
#include <string>
#include <map>

#include <glbinding/callbacks.h>

#include <TacoGL/OpenGL.h>

namespace TacoGL
{

  /**
   * Standard TacoGl error.
   */
  class Error : public std::exception
  {
  public:
    Error() throw() {}
    virtual ~Error() throw() {}

    virtual const char* what() const throw() = 0;
  };

  /**
   * Error used for OpenGL internal errors, retrieved with glGetError.
   */
  class InternalError : public Error
  {
  public:
    /**
     * Get current OpenGl error code.
     * @return The error code.
     *
     * @see glGetError
     */
    static gl::GLenum get();

    static void check(const glbinding::FunctionCall &call);

    InternalError(gl::GLenum code, const std::string &message) throw();

    virtual ~InternalError() throw();

    gl::GLenum getCode() const { return m_code; }
    
    const std::string& getMessage() const { return m_message; }

    virtual const char* what() const throw();

  protected:
    gl::GLenum m_code;
    std::string m_message;
  };

} // end namespace GL

#endif
