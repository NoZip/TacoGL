#ifndef __TACOGL_PROGRAM__
#define __TACOGL_PROGRAM__

#include <string>
#include <vector>
#include <initializer_list>
#include <unordered_map>

#include <TacoGL/OpenGL.h>
#include <TacoGL/Error.h>
#include <TacoGL/algebra.h>

#include <TacoGL/Object.h>
#include <TacoGL/Shader.h>
#include <TacoGL/Texture.h>

namespace TacoGL
{
  /**
   * handle OpenGL programs
   */
  class Program : public Object
  {
  public:
    /**
     * Exception for Program link errors. 
     */
    class LinkError : public Error
    {
    public:
      LinkError(const std::string &log) throw();
      virtual ~LinkError() throw();

      virtual const char* what() const throw();

    protected:
      std::string m_log;
    };

    struct GLSLVariable
    {
      gl::GLint location;
      size_t size;
      gl::GLenum type;
    };

    using AttributeMap = std::unordered_map<std::string, GLSLVariable>;
    using UniformMap = std::unordered_map<std::string, GLSLVariable>;

    Program();
    virtual ~Program();

    const AttributeMap& getActiveAttributes() { return m_activeAttributes; }
    const UniformMap& getActiveUniforms() { return m_activeUniforms; }

    //==================//
    // Shaders managing //
    //==================//

    /**
     * Attach a shader
     * @param shader the shader to attach.
     */
    void attach(const Shader &shader);

    /**
     * Detach a shader.
     * @param shader The shader to detach.
     */
    void detach(const Shader &shader);

    //=================//
    // Linking and use //
    //=================//

    /**
     * Link the program.
     */
    void link();

    /**
     * Get the Program's log. Useful for understanding linking errors.
     */
    std::string getLog();

    /**
     * Make this program the current Program.
     */
    void use();

    //=====================//
    // Location retrieving //
    //=====================//

    /**
     * Retrieve location of a named attribute.
     * @param  name the attribute name.
     * @return      The attribute location if defined, otherwise returns -1.
     * @see glGetAttribLocation
     */
    gl::GLint getAttributeLocation(const std::string &name) const;

    /**
     * Retrieve location of a named uniform
     * @param  name the uniform name.
     * @return      The uniform location if defined, otherwise returns -1.
     * @see glGetUniformLocation
     */
    gl::GLint getUniformLocation(const std::string &name) const;

    //==========//
    // Uniforms //
    //==========//

    void setUniform(const std::string &name, gl::GLfloat value);
    void setUniform(const std::string &name, gl::GLint value);
    void setUniform(const std::string &name, gl::GLuint value);

    void setUniform(const std::string &name, const Vector2 &value);
    void setUniform(const std::string &name, const Vector2i &value);

    void setUniform(const std::string &name, const Vector3 &value);
    void setUniform(const std::string &name, const Vector3i &value);

    void setUniform(const std::string &name, const Vector4 &value);
    void setUniform(const std::string &name, const Vector4i &value);

    void setUniform(const std::string &name, const Matrix2 &value);

    void setUniform(const std::string &name, const Matrix3 &value);

    void setUniform(const std::string &name, const Matrix4 &value);

    void setUniform(const std::string &name, Texture &texture);

    //====================//
    // Program parameters //
    //====================//

    bool getDeleteStatus() const;
    bool getLinkStatus() const;
    bool getValidateStatus() const;
    size_t getInfoLogLength() const;
    size_t getAttachedShadersCount() const;
    size_t getActiveAtomicCounterBuffersLength() const;
    size_t getActiveAttributesCount() const;
    size_t getActiveAttributeMaxLength() const;
    size_t getActiveUniformsCount() const;
    size_t getActiveUniformMaxLength() const;
    size_t getProgramBinaryLength() const;
    std::array<size_t, 3> getComputeWorkGroupSize() const;
    gl::GLenum getTransformFeedbackBufferMode() const;
    size_t getTransformFeedbackVaryingsCount() const;
    size_t getTransformFeedbackVaryingMaxLength() const;
    size_t getGeometryVerticesOut() const;
    gl::GLenum getGeometryInputType() const;
    gl::GLenum getGeometryOutputType() const;

  protected:
    AttributeMap m_activeAttributes;
    UniformMap m_activeUniforms;

    //=================================//
    // Attributes and Uniforms Caching //
    //=================================//

    void computeActiveAttributes();
    void computeActiveUniforms();
  };

} // end namespace GL

#endif
