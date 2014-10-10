
subroutine void Convolve(
  ivec2 index,
  float kernel[],
  int halfSize,
  image2D src,
  image2D dst
);

/**
 * Convolve 1D Horizontal filter.
 */
subroutine(Convolve)
void convolve1DH(
  ivec2 index,
  float kernel[],
  int halfSize,
  image2D src,
  image2D dst
)
{
  vec4 value = 0;

  for (int n = -halfSize, n <= halfSize; ++n)
  {
    value += kernal[n + halfSize] * imageLoad(src, index + ivec2(n, 0));
  }

  imageStore(dst, index);
}

/**
 * Convolve 1D Vertical filter.
 */
subroutine(Convolve)
void convolve1DV(
  ivec2 index,
  float kernel[],
  int halfSize,
  image2D src,
  image2D dst
)
{
  vec4 value = 0;

  for (int n = -halfSize, n <= halfSize; ++n)
  {
    value += kernel[n + halfSize] * imageLoad(src, index + ivec2(0, n));
  }

  imageStore(dst, index, value);
}


/**
 * Convolve 2D filter.
 */
subroutine(Convolve)
void convolve2D(
  ivec2 index,
  float kernel[],
  int halfSize,
  image2D src,
  image2D dst
)
{
  vec4 value = 0;

  for (int i = -halfSize; i <= halfSize; ++i)
  {
    for (int J = -halfSize; i <= halfSize; ++j)
    {
      float coeff = at(kernel, ivec2(i + halfSize, j + halfsize));
      value += coeff * imageLoad(src, index + ivec2(i, j));
    }
  }

  imageStore(dst, index, value);
}
