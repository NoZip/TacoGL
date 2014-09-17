cmake_minimum_required (VERSION 2.6)
project (TacoGL)

set(CMAKE_CXX_FLAGS  "-std=c++11")

include_directories(BEFORE "${CMAKE_CURRENT_SOURCE_DIR}/include")

# add_definitions("-DGLBINDING_STATIC")

find_package(PkgConfig REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glbinding REQUIRED)
# find_package(glfw REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/source")

set(TACOGL_SRCS_DIR "${SOURCE_DIR}/TacoGL")
set(TACOGL_SRCS
    "${TACOGL_SRCS_DIR}/Error.cpp"
    "${TACOGL_SRCS_DIR}/Buffer.cpp"
    "${TACOGL_SRCS_DIR}/Texture.cpp"
    "${TACOGL_SRCS_DIR}/Sampler.cpp"
    "${TACOGL_SRCS_DIR}/Shader.cpp"
    "${TACOGL_SRCS_DIR}/Program.cpp"
    "${TACOGL_SRCS_DIR}/VertexArray.cpp"
    "${TACOGL_SRCS_DIR}/AttributeFormat.cpp"
    "${TACOGL_SRCS_DIR}/Framebuffer.cpp"
    "${TACOGL_SRCS_DIR}/Renderbuffer.cpp"
)

add_library(TacoGL ${TACOGL_SRCS})

set(TEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test")

add_executable(test_gl "${TEST_DIR}/test_gl.cpp")
target_link_libraries(test_gl
    ${OPENGL_LIBRARIES}
    ${GLBINDING_LIBRARIES}
    ${GLFW_STATIC_LIBRARIES}
    SOIL
    TacoGL
)
