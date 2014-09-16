#include <fstream>
#include <cassert>

#include <TacoGL/Shader.h>

using namespace gl;
using namespace TacoGL;

Shader::CompilationError::CompilationError(const std::string &log) throw()
: m_log(log)
{

}

Shader::CompilationError::~CompilationError() throw()
{

}

const char * Shader::CompilationError::what() const throw()
{
  return m_log.c_str();
}

Shader::Shader(GLenum type)
{
  m_id = glCreateShader(type);
}

Shader::Shader(GLenum type, const std::string &filename) : Shader(type)
{
  setSource(filename);
  compile();
}

Shader::~Shader() {
  glDeleteShader(m_id);
}

void Shader::compile() {
  glCompileShader(m_id);

  if (!getCompileStatus())
  {
    throw CompilationError(getLog());
  }
}

void Shader::setSource(
  GLsizei sourceLength,
  const GLint *lineLength,
  const GLchar * const *source
)
{
  glShaderSource(m_id, sourceLength, source, lineLength);
}

void Shader::setSource(std::ifstream &stream)
{
    std::vector<std::string> source;

    loadSourceFromStream(stream, source);

    assert(!source.empty());

    const GLchar *internalSource[source.size()];
    GLint lineLength[source.size()];

    for (int i = 0; i < source.size(); ++i)
    {
        internalSource[i] = source[i].c_str();
        lineLength[i] = source[i].size();
    }

    setSource(source.size(), lineLength, internalSource);
}

void Shader::setSource(const std::string &filename)
{
    std::ifstream file(filename);
    setSource(file);
}

std::string Shader::getLog() const
{
    GLint logLength = getInfoLogLength();

    GLchar log[logLength];

    glGetShaderInfoLog(m_id, logLength, nullptr, log);

    return std::string(log);
}

void Shader::loadSourceFromStream(
    std::istream &stream,
    std::vector<std::string> &source
)
{
    std::string line;
    while (getline(stream, line))
    {
        line += '\n';
        source.push_back(line);
    }
}

template<GLenum PARAMETER, typename T>
inline T getShader(GLuint id)
{
    GLint value;
    glGetShaderiv(id, PARAMETER, &value);
    return static_cast<T>(value);
}

GLenum Shader::getShaderType() const
{
return getShader<GL_SHADER_TYPE, GLenum>(m_id);
}

bool Shader::getDeleteStatus() const
{
return getShader<GL_DELETE_STATUS, bool>(m_id);
}

bool Shader::getCompileStatus() const
{
return getShader<GL_COMPILE_STATUS, bool>(m_id);
}

size_t Shader::getInfoLogLength() const
{
return getShader<GL_INFO_LOG_LENGTH, int>(m_id);
}

size_t Shader::getShaderSourceLength() const
{
return getShader<GL_SHADER_SOURCE_LENGTH, int>(m_id);
}
