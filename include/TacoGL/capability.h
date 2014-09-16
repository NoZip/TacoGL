#ifndef __TACOGL_CAPABILITY_REPOSITORY__
#define __TACOGL_CAPABILITY_REPOSITORY__

#include <TacoGL/OpenGL.h>
#include <TacoGL/Error.h>

#include <TacoGL/capabilities/Capability.h>
#include <TacoGL/capabilities/Blend.h>
#include <TacoGL/capabilities/ColorLogicOperator.h>
#include <TacoGL/capabilities/DepthTest.h>

namespace TacoGL
{

  // Blend: see <TacoGL/capabilities/Blend.h>
  // TODO: gl::GL_CLIP_DISTANCE i
  // ColorLogicOpertaor: see <TacoGL/capabilities/ColorLogicOperator.h>
  using CullFace = Capability<gl::GL_CULL_FACE>;
  using DebugOutput = Capability<gl::GL_DEBUG_OUTPUT>;
  using DebugOutputSynchronous = Capability<gl::GL_DEBUG_OUTPUT_SYNCHRONOUS>;
  using DepthClamp = Capability<gl::GL_DEPTH_CLAMP>;
  // DepthTest: see <TacoGL/capabilities/DepthTest.h>
  using Dither = Capability<gl::GL_DITHER>;
  using FrameBufferSRGB = Capability<gl::GL_FRAMEBUFFER_SRGB>;
  using LineSmooth = Capability<gl::GL_LINE_SMOOTH>;
  using PolygonOffsetFill = Capability<gl::GL_POLYGON_OFFSET_FILL>;
  using PolygonOffsetLine = Capability<gl::GL_POLYGON_OFFSET_LINE>;
  using PolygonOffsetPoint = Capability<gl::GL_POLYGON_OFFSET_POINT>;
  using PolygonSmooth = Capability<gl::GL_POLYGON_SMOOTH>;
  using PrimitiveRestart = Capability<gl::GL_PRIMITIVE_RESTART>;
  using PrimitiveRestartFixedIndex = Capability<gl::GL_PRIMITIVE_RESTART_FIXED_INDEX>;
  using RasterizerDiscard = Capability<gl::GL_RASTERIZER_DISCARD>;
  using SampleAlphaToCoverage = Capability<gl::GL_SAMPLE_ALPHA_TO_COVERAGE>;
  using SampleAlphaToOne = Capability<gl::GL_SAMPLE_ALPHA_TO_ONE>;
  using SampleCoverage = Capability<gl::GL_SAMPLE_COVERAGE>;
  using SampleShading = Capability<gl::GL_SAMPLE_SHADING>;
  using SampleMask = Capability<gl::GL_SAMPLE_MASK>;
  using ScissorTest = Capability<gl::GL_SCISSOR_TEST>;
  using StencilTest = Capability<gl::GL_STENCIL_TEST>;
  using TextureCubeMapSeamless = Capability<gl::GL_TEXTURE_CUBE_MAP_SEAMLESS>;
  using ProgramPointSize = Capability<gl::GL_PROGRAM_POINT_SIZE>;

} // end namespace GL

#endif
