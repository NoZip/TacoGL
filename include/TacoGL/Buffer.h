#ifndef __TACOGL_BUFFER__
#define __TACOGL_BUFFER__

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <iterator>
#include <algorithm>

#include <TacoGL/OpenGL.h>
#include <TacoGL/Error.h>
#include <TacoGL/Object.h>

namespace TacoGL
{

  /**
   * This class is designed to use GPU memory OpenGL buffers.
   */
  class Buffer : public Object
  {
  public:

    /**
     * Manages buffer bindings.
     */
    class Manager
    {
    public:
      using BindedTargetMap = std::unordered_set<gl::GLenum>;
      using BindingMap = std::unordered_map<gl::GLuint, gl::GLenum>;

      Manager();

      virtual ~Manager();

      const BindedTargetMap & getTarget() const;
      const BindingMap & getBinding() const;
      gl::GLenum getBinding(gl::GLuint bufferId) const;

      void bind(gl::GLenum target, gl::GLuint bufferId);

      void unbind(gl::GLuint bufferId);
      
      void debug() const;

    protected:
      BindedTargetMap m_target;
      BindingMap m_binding;
    };

    Buffer();

    virtual ~Buffer();

    size_t getSize() const { return m_size; }

    /**
     * Binds the buffer in the current OpenGl context.
     *
     * @param target the target to bind the buffer to.
     * @see glBindBuffer
     */
    void bind(gl::GLenum target);

    /**
     * Unbinds the buffer in the current OpenGl context.
     */
    void unbind();

    /**
     * Allocate mutable storage in GPU memory.
     * 
     * @tparam T datatype to allocate.
     * @param size the number of T elemnts to allocate.
     * @param usage OpenGL memory usage.
     * @see glBufferData
     */
    template <typename T>
    void allocate(size_t size, gl::GLenum usage, const T* data = nullptr);


    /**
     * Allocate Immutable storage in GPU memory.
     * 
     * @tparam T datatype to allocate.
     * @param size the number of T elemnts to allocate.
     * @param flags OpenGL flags for persistent storage.
     * @see glBufferStorage
     */
    template <typename T>
    void allocate(size_t size, gl::MapBufferUsageMask flags, const T* data = nullptr);


    /**
     * Update data in GPU memory.
     * 
     * @param first an iterator to read from.
     * @see glBufferSubData
     */
    template <typename InputIterator>
    void set(const InputIterator first);

    /**
     * Update data in GPU memory.
     * 
     * @param first an iterator to read from.
     * @param count the number of elements to set.
     * @param offset the offset from buffer start.
     * @see glBufferSubData
     */
    template <typename InputIterator>
    void set(const InputIterator first, size_t count, size_t offset);


    /**
     * Retrieve data from GPU memory.
     * 
     * @param first an iterator to write into.
     * @see glGetBufferSubData
     */
    template <typename OutputIterator>
    void get(OutputIterator first) const;

    /**
     * Retrieve data from GPU memory.
     * 
     * @param first an iterator to write into.
     * @param count the number of elements to get.
     * @param offset the offset from buffer start.
     * @see glGetBufferSubData
     */
    template <typename OutputIterator>
    void get(OutputIterator first, size_t count, size_t offset) const;

  protected:
    static Manager s_manager; ///< buffer bindings manager.

    size_t m_size; ///< the buffer size in memory.
  };

  #include <TacoGL/Buffer.hpp>

} // end namespace GL

#endif
