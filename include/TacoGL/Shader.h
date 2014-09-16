#ifndef __TACOGL_SHADER__
#define __TACOGL_SHADER__

#include <string>
#include <vector>
#include <istream>

#include <TacoGL/OpenGL.h>

#include <TacoGL/Error.h>
#include <TacoGL/Object.h>

namespace TacoGL
{

  /**
   * Handle OpenGL shader operations.
   */
  class Shader : public Object
  {
  public:
    /**
     * Exception for shader compilation errors handling.
     */
    class CompilationError : public Error
    {
    public:
      CompilationError(const std::string &log) throw();
      virtual ~CompilationError() throw();

      virtual const char* what() const throw();

    protected:
      std::string m_log;
    };

    /**
     * Enum for shader type.
     * @see [7.1-2] of opengl spec.
     */
    // enum class Type : GLenum
    // {
    //   VERTEX          = GL_VERTEX_SHADER,
    //   FRAGMENT        = GL_FRAGMENT_SHADER,
    //   GEOMETRY        = GL_GEOMETRY_SHADER,
    //   TESS_EVALUATION = GL_TESS_EVALUATION_SHADER,
    //   TESS_CONTROL    = GL_TESS_CONTROL_SHADER,
    //   COMPUTE         = GL_COMPUTE_SHADER
    // };

    /**
     * Default constructor.
     * @param type The shader Type.
     */
    Shader(gl::GLenum type);

    /**
     * Initialize a shader from a file.
     * @param type     The shader type.
     * @param filename The file to read the source from.
     */
    Shader(gl::GLenum type, const std::string &filename);

    /**
     * Destructor
     */
    virtual ~Shader();

    /**
     * Set shader source from raw data.
     * @param sourceLength Source size.
     * @param lineLength   An array of line size.
     * @param source       The source in an array of lines.
     */
    void setSource(
      gl::GLsizei sourceLength,
      const gl::GLint *lineLength,
      const gl::GLchar * const *source
    );

    /**
     * Load source from stream.
     * @param stream The stream to read source from.
     */
    void setSource(std::ifstream &stream);

    /**
     * Load source from file.
     * @param filename File to load.
     */
    void setSource(const std::string &filename);

    /**
     * Compile the shader.
     */
    void compile();

    /**
     * Fetch compilation log. useful for debug.
     * @return A string describing the compilation error.
     */
    std::string getLog() const;

    gl::GLenum getShaderType() const;
    bool getDeleteStatus() const;
    bool getCompileStatus() const;
    size_t getInfoLogLength() const;
    size_t getShaderSourceLength() const;


   protected:
    /**
     * Lod source from a stream into a vector of string.
     * @param stream The stream to read from.
     * @param source the vector to write into.
     */
    static void loadSourceFromStream(
      std::istream &stream,
      std::vector<std::string> &source
    );
  };

} // end namespace GL

#endif
