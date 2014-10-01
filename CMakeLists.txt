cmake_minimum_required (VERSION 2.6)
project (TacoGL)

# add_definitions("-DGLBINDING_STATIC")

find_package(PkgConfig REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glbinding REQUIRED)
find_package(Eigen3 REQUIRED)
# find_package(glfw REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

set(CMAKE_CXX_FLAGS  "-std=c++11")

set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/")
set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/source/")

include_directories(${EIGEN3_INCLUDE_DIR})
include_directories(${INCLUDE_DIR})

set(TACOGL_INC_DIR "${INCLUDE_DIR}/TacoGL/")
file(GLOB TACOGL_INCS "${TACOGL_INC_DIR}/*.h" "${TACOGL_INC_DIR}/*.hpp")
file(GLOB TACOGL_CAPABILITIES_INCS 
    "${TACOGL_INC_DIR}/capabilities/*.h" 
    "${TACOGL_INC_DIR}/capabilities/*.hpp"
)

set(TACOGL_SRC_DIR "${SOURCE_DIR}/TacoGL/")
set(TACOGL_SRCS
    "${TACOGL_SRC_DIR}/Error.cpp"
    "${TACOGL_SRC_DIR}/ExtensionRegister.cpp"
    "${TACOGL_SRC_DIR}/Buffer.cpp"
    "${TACOGL_SRC_DIR}/Texture.cpp"
    "${TACOGL_SRC_DIR}/Sampler.cpp"
    "${TACOGL_SRC_DIR}/Shader.cpp"
    "${TACOGL_SRC_DIR}/Program.cpp"
    "${TACOGL_SRC_DIR}/VertexArray.cpp"
    "${TACOGL_SRC_DIR}/AttributeFormat.cpp"
    "${TACOGL_SRC_DIR}/Framebuffer.cpp"
    "${TACOGL_SRC_DIR}/Renderbuffer.cpp"
)

add_library(TacoGL ${TACOGL_SRCS})

install(FILES ${TACOGL_INCS} DESTINATION include/TacoGL/)
install(FILES ${TACOGL_CAPABILITIES_INCS} DESTINATION include/TacoGL/capabilities/)

install(TARGETS TacoGL
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

set(TEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test/")

add_executable(test_gl "${TEST_DIR}/test_gl.cpp")
target_link_libraries(test_gl
    ${OPENGL_LIBRARIES}
    ${GLBINDING_LIBRARIES}
    ${GLFW_STATIC_LIBRARIES}
    SOIL
    TacoGL
)
