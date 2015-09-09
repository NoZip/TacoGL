#ifndef __TACOGL_SHADER__
#define __TACOGL_SHADER__

#include <string>
#include <vector>
#include <list>
#include <unordered_set>
#include <unordered_map>
#include <istream>

#include <TacoGL/OpenGL.h>

#include <TacoGL/Error.h>
#include <TacoGL/Object.h>

namespace TacoGL
{
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

  using GLSLSource = std::vector<std::string>; 

  class ShaderFinder
  {
  public:
    using DirectoryList = std::list<std::string>;

    ShaderFinder() = default;
    ShaderFinder(const DirectoryList &directories);
    virtual ~ShaderFinder() = default;

    void addDirectory(const std::string &directory);

    std::string find(const std::string &filename) const;

  protected:
    DirectoryList m_directories;
  };

  class SourceLoader
  {
  public:
    using IncludeSet = std::unordered_set<std::string>;
    using DefineMap = std::unordered_map<std::string, std::string>;

    SourceLoader(const ShaderFinder &manager);
    virtual ~SourceLoader() = default;

    void load(
      const std::string &filename,
      GLSLSource &source,
      const DefineMap &defines = DefineMap()
    );

  protected:
    const ShaderFinder &m_finder;
    IncludeSet m_includes;
  };

  /**
   * Handle OpenGL shader operations.
   */
  class Shader : public Object
  {
  public:
    static ShaderFinder & getFinder();

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

    void setSource(const GLSLSource &source);

    void setSource(
      const std::string &filename,
      const SourceLoader::DefineMap &defines = SourceLoader::DefineMap()
    );

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
    static ShaderFinder s_finder;
  };

} // end namespace GL

#endif
