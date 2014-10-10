#version 430 core
#extension ARB_shader_image_load_store : require
#extension ARB_shader_subroutine       : require

layout(local_size_x = 32, local_size_y = 32) in; // max size 1024 = 32*32

subroutine void Convolve(
  ivec2 index,
  float kernel[],
  int halfSize,
  image2D src,
  image2D dst
);

uniform readonly image2D src;
uniform writeonly image2D dst;

uniform int kernelHalfSize;

subroutine uniform Convolve convolve;

/**
 * Defined in row-major order.
 * Here is an example for a 2D kernel size of 3:
 * 
 * |===|===|===|
 * | 0 | 1 | 2 |
 * |---|---|---|
 * | 3 | 4 | 5 |
 * |---|---|---|
 * | 6 | 7 | 8 |
 * |===|===|===|
 *
 * 1D kernels are simplier:
 * 
 * |===|===|===|
 * | 0 | 1 | 2 |
 * |===|===|===|
 */
buffer float kernel[];

/**
 * Fetches a value in a 2D array.
 * @param index The index to fetch.
 */
float at(float kernel[], ivec2 index)
{
  return kernel[index.x*kernelSize + index.y];
}

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

void main()
{
  ivec2 index = gl_GlobalInvocationID.xy;
  ivec2 size = imageSize(dst);

  // checks if the index is in the image.
  if (index.x > size.x || index.y > size.y)
  {
    return;
  }

  convolve(index, kernel, kernelHalfSize, src, dst);
}
