
template <typename T>
void Buffer::allocate(size_t size, gl::GLenum usage, const T* data)
{
  assert(isBinded());

  gl::GLenum target = getTarget();
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
  assert(isBinded());

  gl::GLenum target = getTarget();
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
  assert(isBinded());

  using value_t = typename std::iterator_traits<InputIterator>::value_type;

  size_t count = m_size / sizeof(value_t);

  std::vector<value_t> array(count);

  std::copy_n(first, count, array.begin());

  glBufferSubData(
    getTarget(),
    0,
    m_size,
    static_cast<const void*>(array.data())
  );
}

template <typename InputIterator>
void Buffer::set(const InputIterator first, size_t count, size_t offset)
{
  assert(isBinded());

  using value_t = typename std::iterator_traits<InputIterator>::value_type;

  size_t size = count * sizeof(value_t);
  offset *= sizeof(value_t);

  std::vector<value_t> array(count);

  std::copy_n(first, count, array.begin());

  glBufferSubData(
    getTarget(),
    offset,
    size,
    static_cast<const void*>(array.data())
  );
}

template <typename OutputIterator>
void Buffer::get(OutputIterator first) const
{
  assert(isBinded());

  using value_t = typename std::iterator_traits<OutputIterator>::value_type;

  size_t count = m_size / sizeof(value_t);

  std::vector<value_t> array(count);

  glGetBufferSubData(
    getTarget(),
    0,
    m_size,
    static_cast<void *>(array.data())
  );

  std::copy_n(array.begin(), count, first);
}

template <typename OutputIterator>
void Buffer::get(OutputIterator first, size_t count, size_t offset) const
{
  assert(isBinded());

  using value_t = typename std::iterator_traits<OutputIterator>::value_type;

  size_t size = count * sizeof(value_t);
  offset *= sizeof(value_t);

  std::vector<value_t> array(count);

  glGetBufferSubData(
    getTarget(),
    offset,
    size,
    static_cast<void *>(array.data())
  );

  std::copy_n(array.begin(), count, first);
}
