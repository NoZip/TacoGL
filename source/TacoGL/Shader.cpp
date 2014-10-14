#include <cassert>
#include <fstream>
#include <regex>

#include <TacoGL/Shader.h>

using namespace gl;
using namespace TacoGL;

CompilationError::CompilationError(const std::string &log) throw()
: m_log(log)
{

}

CompilationError::~CompilationError() throw()
{

}

const char * CompilationError::what() const throw()
{
  return m_log.c_str();
}

ShaderFinder::ShaderFinder(const ShaderFinder::DirectoryList &directories)
: m_directories(directories)
{

}

void ShaderFinder::addDirectory(const std::string &directory)
{
  m_directories.push_back(directory);
}

std::string ShaderFinder::find(const std::string &filename) const
{
  for (auto &directory : m_directories)
  {
    std::string path = directory + filename;
    std::ifstream input(path);
    if (input.is_open())
    {
      input.close();
      return path;
    }
  }

  return "not found";
}

SourceLoader::SourceLoader(const ShaderFinder &finder)
: m_finder(finder), m_includes()
{

}

namespace
{
  std::regex includeRegex("#include\\s+(.+)");
}

void SourceLoader::load(
  const std::string &filename,
  GLSLSource &source,
  const SourceLoader::DefineMap &defines
)
{
  for (auto &definePair : defines)
  {
    source.push_back("#define " + definePair.first + " " + definePair.second + "\n");
  }

  std::ifstream input(m_finder.find(filename));

  std::string line;
  std::smatch matchGroups;
  while (std::getline(input, line))
  {
    if (std::regex_match(line, matchGroups, includeRegex))
    {
      std::string include = matchGroups[1];

      if (m_includes.find(include) != m_includes.end())
      {
        continue;
      }

      m_includes.insert(include);

      load(include, source);

      continue;
    }
    
    line += '\n';
    source.push_back(line);
  }
}

ShaderFinder & Shader::getFinder()
{
  return s_finder;
}

ShaderFinder Shader::s_finder;

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

void Shader::setSource(const GLSLSource &source)
{
  assert(!source.empty());

  const GLchar **internalSource = new const GLchar*[source.size()];
  GLint *lineLength = new GLint[source.size()];

  for (int i = 0; i < source.size(); ++i)
  {
      internalSource[i] = source[i].c_str();
      lineLength[i] = source[i].size();
  }

  glShaderSource(m_id, source.size(), internalSource, lineLength);

  delete[] internalSource;
  delete[] lineLength;
}

void Shader::setSource(
  const std::string &filename,
  const SourceLoader::DefineMap &defines
)
{  
  GLSLSource source;
  SourceLoader(s_finder).load(filename, source, defines);

  setSource(source);
}

std::string Shader::getLog() const
{
    GLint logLength = getInfoLogLength();

    GLchar *rawLog = new GLchar[logLength];

    glGetShaderInfoLog(m_id, logLength, nullptr, rawLog);

    std::string log(rawLog);

    return log;
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
