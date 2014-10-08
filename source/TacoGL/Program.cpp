#include <cassert>

#include <TacoGL/Program.h>

using namespace gl;
using namespace TacoGL;

//============//
// Link Error //
//============//

Program::LinkError::LinkError(const std::string &log) throw()
: m_log(log)
{

}

Program::LinkError::~LinkError() throw()
{

}

const char * Program::LinkError::what() const throw()
{
  return m_log.c_str();
}

//=========//
// Program //
//=========//

Program::Program()
{
  m_id = glCreateProgram();
}

Program::~Program()
{
  glDeleteProgram(m_id);
}

//--------------------//
// Shaders Management //
//--------------------//

void Program::attach(const Shader &shader)
{
  glAttachShader(m_id, shader.getId());
}

void Program::detach(const Shader &shader)
{
  glDetachShader(m_id, shader.getId());
}

//-----------------//
// Linking and use //
//-----------------//

void Program::link()
{
  glLinkProgram(m_id);

  if (!getLinkStatus())
  {
    throw LinkError(getLog());
  }

  computeActiveAttributes();
  computeActiveUniforms();
}

std::string Program::getLog()
{
  GLint logSize = getInfoLogLength();

  GLchar *rawLog = new GLchar[logSize];

  glGetProgramInfoLog(m_id, logSize, nullptr, rawLog);

  std::string log(rawLog);

  delete[] rawLog;

  return log;
}

void Program::use()
{
  glUseProgram(m_id);
}

//-------------------//
// Retrieve Location //
//-------------------//

GLint Program::getAttributeLocation(const std::string &name) const
{
  if (m_activeAttributes.find(name) != m_activeAttributes.end())
    return m_activeAttributes.at(name).location;
  else
    return -1;
}

GLint Program::getUniformLocation(const std::string &name) const
{
  if (m_activeUniforms.find(name) != m_activeUniforms.end())
    return m_activeUniforms.at(name).location;
  else
    return -1;
}

//-------------------//
// Unniforms Setters //
//-------------------//

void Program::setUniform(const std::string &name, GLfloat value)
{
  GLint location = getUniformLocation(name);
  glProgramUniform1f(m_id, location, value);
}

void Program::setUniform(const std::string &name, GLint value)
{
  GLint location = getUniformLocation(name);
  glProgramUniform1i(m_id, location, value);
}

void Program::setUniform(const std::string &name, GLuint value)
{
  GLint location = getUniformLocation(name);
  glProgramUniform1ui(m_id, location, value);
}

void Program::setUniform(const std::string &name, const Vector2 &value)
{
  GLint location = getUniformLocation(name);
  glProgramUniform2fv(m_id, location, 1, value.data());
}

void Program::setUniform(const std::string &name, const Vector2i &value)
{
  GLint location = getUniformLocation(name);
  glProgramUniform2iv(m_id, location, 1, value.data());
}

void Program::setUniform(const std::string &name, const Vector3 &value)
{
  GLint location = getUniformLocation(name);
  glProgramUniform3fv(m_id, location, 1, value.data());
}

void Program::setUniform(const std::string &name, const Vector3i &value)
{
  GLint location = getUniformLocation(name);
  glProgramUniform3iv(m_id, location, 1, value.data());
}

void Program::setUniform(const std::string &name, const Vector4 &value)
{
  GLint location = getUniformLocation(name);
  glProgramUniform4fv(m_id, location, 1, value.data());
}

void Program::setUniform(const std::string &name, const Vector4i &value)
{
  GLint location = getUniformLocation(name);
  glProgramUniform4iv(m_id, location, 1, value.data());
}

void Program::setUniform(const std::string &name, const Matrix2 &value)
{
  GLint location = getUniformLocation(name);
  glProgramUniformMatrix2fv(m_id, location, 1, GL_FALSE, value.data());
}

void Program::setUniform(const std::string &name, const Matrix3 &value)
{
  GLint location = getUniformLocation(name);
  glProgramUniformMatrix3fv(m_id, location, 1, GL_FALSE, value.data());
}

void Program::setUniform(const std::string &name, const Matrix4 &value)
{
  GLint location = getUniformLocation(name);
  glProgramUniformMatrix4fv(m_id, location, 1, GL_FALSE, value.data());
}

void Program::setUniform(
  const std::string &name,
  Texture &texture
)
{
  GLint location = getUniformLocation(name);
  size_t unit = Texture::getTextureUnitManager().getUnitBinding(texture.getId());
  glProgramUniform1i(m_id, location, static_cast<GLint>(unit));
}

//--------------------//
// Program Parameters //
//--------------------//

namespace
{
  template<GLenum PARAMETER, typename T>
  inline T getProgram(GLuint id)
  {
    GLint data;
    glGetProgramiv(id, PARAMETER, &data);
    return static_cast<T>(data);
  }

  template<GLenum PARAMETER, typename T>
  inline T getProgramv(GLuint id)
  {
    T vector;
    GLint *data = new GLint[vector.size()];
    glGetProgramiv(id, PARAMETER, data);

    std::copy_n(data, vector.size(), vector.data());

    delete[] data;

    return vector;
  } 
}

bool Program::getDeleteStatus() const
{
  return getProgram<GL_DELETE_STATUS, bool>(m_id);
}

bool Program::getLinkStatus() const
{
  return getProgram<GL_LINK_STATUS, bool>(m_id);
}

bool Program::getValidateStatus() const
{
  return getProgram<GL_VALIDATE_STATUS, bool>(m_id);
}

size_t Program::getInfoLogLength() const
{
  return getProgram<GL_INFO_LOG_LENGTH, size_t>(m_id);
}

size_t Program::getAttachedShadersCount() const
{
  return getProgram<GL_ATTACHED_SHADERS, size_t>(m_id);
}

size_t Program::getActiveAtomicCounterBuffersLength() const
{
  return getProgram<GL_ACTIVE_ATOMIC_COUNTER_BUFFERS, size_t>(m_id);
}

size_t Program::getActiveAttributesCount() const
{
  return getProgram<GL_ACTIVE_ATTRIBUTES, size_t>(m_id);
}

size_t Program::getActiveAttributeMaxLength() const
{
  return getProgram<GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, size_t>(m_id);
}

size_t Program::getActiveUniformsCount() const
{
  return getProgram<GL_ACTIVE_UNIFORMS, size_t>(m_id);
}

size_t Program::getActiveUniformMaxLength() const
{
  return getProgram<GL_ACTIVE_UNIFORM_MAX_LENGTH, size_t>(m_id);
}

size_t Program::getProgramBinaryLength() const
{
  return getProgram<GL_PROGRAM_BINARY_LENGTH, size_t>(m_id);
}

std::array<size_t, 3> Program::getComputeWorkGroupSize() const
{
  return getProgramv<GL_COMPUTE_WORK_GROUP_SIZE, std::array<size_t, 3>>(m_id);
}

GLenum Program::getTransformFeedbackBufferMode() const
{
  return getProgram<GL_TRANSFORM_FEEDBACK_BUFFER_MODE, GLenum>(m_id);
}

size_t Program::getTransformFeedbackVaryingsCount() const
{
  return getProgram<GL_TRANSFORM_FEEDBACK_VARYINGS, size_t>(m_id);
}

size_t Program::getTransformFeedbackVaryingMaxLength() const
{
  return getProgram<GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH, size_t>(m_id);
}

size_t Program::getGeometryVerticesOut() const
{
  return getProgram<GL_GEOMETRY_VERTICES_OUT, size_t>(m_id);
}

GLenum Program::getGeometryInputType() const
{
  return getProgram<GL_GEOMETRY_INPUT_TYPE, GLenum>(m_id);
}

GLenum Program::getGeometryOutputType() const
{
  return getProgram<GL_GEOMETRY_OUTPUT_TYPE, GLenum>(m_id);
}

//---------------------------------//
// Attributes and Uniforms Caching //
//---------------------------------//

void Program::computeActiveAttributes()
{
  m_activeAttributes.clear();

  GLint count = getActiveAttributesCount();

  GLint bufferSize = getActiveAttributeMaxLength();

  GLsizei size = 0;
  GLchar *name = new GLchar[bufferSize];
  GLenum type;
  for (int i = 0; i < count; ++i)
  {
      glGetActiveAttrib(m_id, i, bufferSize, nullptr, &size, &type, name);
      m_activeAttributes.emplace(
        std::string(name),
        GLSLVariable{glGetAttribLocation(m_id, name), static_cast<size_t>(size), type}
      );
  }

  delete[] name;
}

void Program::computeActiveUniforms()
{
  m_activeUniforms.clear();

  GLint count = getActiveUniformsCount();

  GLint bufferSize = getActiveUniformMaxLength();

  GLsizei size = 0;
  GLchar *name = new GLchar[bufferSize];
  GLenum type;
  for (int i = 0; i < count; ++i)
  {
      glGetActiveUniform(m_id, i, bufferSize, nullptr, &size, &type, name);
      m_activeUniforms.emplace(
        std::string(name),
        GLSLVariable{glGetUniformLocation(m_id, name), static_cast<size_t>(size), type}
      );
  }

  delete[] name;
}

