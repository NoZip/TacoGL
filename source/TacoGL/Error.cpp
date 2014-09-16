#include <glbinding/Meta.h>

#include <TacoGL/Error.h>

using namespace gl;
using namespace glbinding;
using namespace TacoGL;

GLenum InternalError::get()
{
  return glGetError();
}

void InternalError::check(const glbinding::FunctionCall &call)
{
  GLenum error = get();
  if (error != GL_NO_ERROR)
  {
    std::string message = "";

    message += call.function.name();

    message += "(";
    for (int i = 0; i < call.parameters.size(); ++i)
    {
      message += call.parameters[i]->asString();
      if (i < call.parameters.size() - 1)
        message += ", ";
    }
    message += ")";

    if (call.returnValue)
    {
      message += " -> ";
      message += call.returnValue->asString();
    }

    throw InternalError(error, message);
  }
}

InternalError::InternalError(GLenum code, const std::string &message) throw()
: m_code(code), m_message(message)
{

}

InternalError::~InternalError() throw()
{

}

const char* InternalError::what() const throw()
{
    return (Meta::getString(m_code) + " : " + m_message).c_str();
}
