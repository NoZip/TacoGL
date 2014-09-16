
template <typename T>
void Buffer::allocate(size_t size, gl::GLenum usage, const T* data)
{
  gl::GLenum target = s_manager.getBinding(m_id);
  m_size = size * sizeof(T);

  gl::glBufferData(
    target,
    m_size,
    static_cast<const void*>(data),
    usage
  );
}

template <typename T>
void Buffer::allocate(size_t size, gl::MapBufferUsageMask flags, const T* data)
{
  gl::GLenum target = s_manager.getBinding(m_id);
  m_size = size * sizeof(T);

  gl::glBufferStorage(
    target,
    m_size,
    static_cast<const void*>(data),
    flags
  );
}

template <typename InputIterator>
void Buffer::set(const InputIterator first)
{
  using value_t = typename std::iterator_traits<InputIterator>::value_type;

  size_t count = m_size / sizeof(value_t);

  std::vector<value_t> array(count);

  std::copy_n(first, count, array.begin());

  glBufferSubData(
    s_manager.getBinding(m_id),
    0,
    m_size,
    static_cast<const void*>(array.data())
  );
}

template <typename InputIterator>
void Buffer::set(const InputIterator first, size_t count, size_t offset)
{
  using value_t = typename std::iterator_traits<InputIterator>::value_type;

  size_t size = count * sizeof(value_t);
  offset *= sizeof(value_t);

  std::vector<value_t> array(count);

  std::copy_n(first, count, array.begin());

  glBufferSubData(
    s_manager.getBinding(m_id),
    offset,
    size,
    static_cast<const void*>(array.data())
  );
}

template <typename OutputIterator>
void Buffer::get(OutputIterator first) const
{
  using value_t = typename std::iterator_traits<OutputIterator>::value_type;

  size_t count = m_size / sizeof(value_t);

  std::vector<value_t> array(count);

  glGetBufferSubData(
    s_manager.getBinding(m_id),
    0,
    m_size,
    static_cast<void *>(array.data())
  );

  std::copy_n(array.begin(), count, first);
}

template <typename OutputIterator>
void Buffer::get(OutputIterator first, size_t count, size_t offset) const
{
  using value_t = typename std::iterator_traits<OutputIterator>::value_type;

  size_t size = count * sizeof(value_t);
  offset *= sizeof(value_t);

  std::vector<value_t> array(count);

  glGetBufferSubData(
    s_manager.getBinding(m_id),
    offset,
    size,
    static_cast<void *>(array.data())
  );

  std::copy_n(array.begin(), count, first);
}
