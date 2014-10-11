#version 430 core
#extension GL_ARB_shader_image_load_store : require

#ifndef WORKGROUP_SIZE_X
#define WORKGROUP_SIZE_X 32
#endif

#ifndef WORKGROUP_SIZE_Y
#define WORKGROUP_SIZE_Y 32
#endif

#ifndef KERNEL_HALFSIZE
#define KERNEL_HALFSIZE 1
#endif

#define KERNEL_SIZE (2*KERNEL_HALFSIZE + 1)

#ifndef GREYSCALE
#define FORMAT rgba8
#else
#define FORMAT r8
#endif


layout(local_size_x = WORKGROUP_SIZE_X, local_size_y = WORKGROUP_SIZE_Y) in;

uniform uvec2 imageSize;

layout(FORMAT, binding = 0) uniform image2D src;
layout(FORMAT, binding = 1) uniform image2D dst;

uniform float kernel[KERNEL_SIZE*KERNEL_SIZE];

/**
 * Fetches a value in a 2D array.
 * @param index The index to fetch.
 */
float at(float kernel[KERNEL_SIZE*KERNEL_SIZE], ivec2 index)
{
  return kernel[index.x*3 + index.y];
}

void main()
{
  uvec2 index = gl_GlobalInvocationID.xy;

  // checks if the index is in the image.
  if (index.x > imageSize.x || index.y > imageSize.y)
  {
    return;
  }

  vec4 value = vec4(0);

  for (int i = -KERNEL_HALFSIZE; i <= KERNEL_HALFSIZE; ++i)
  {
    for (int j = -KERNEL_HALFSIZE; j <= KERNEL_HALFSIZE; ++j)
    {
      ivec2 kernelOffset = ivec2(i + KERNEL_HALFSIZE, j + KERNEL_HALFSIZE);
      float coeff = kernel[kernelOffset.x*KERNEL_SIZE + kernelOffset.y];
      ivec2 imageOffset = ivec2(i, j);
      value += coeff * imageLoad(src, ivec2(index) + imageOffset);
    }
  }

  clamp(value, 0, 1);

  imageStore(dst, ivec2(index), value);
}
