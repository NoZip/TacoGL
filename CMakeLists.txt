cmake_minimum_required (VERSION 2.8.12 FATAL_ERROR)
project (TacoGL)

set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/source")
set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib")

# add_definitions(-DGLBINDING_STATIC)

add_definitions(-DSHADER_DIR="${CMAKE_CURRENT_SOURCE_DIR}/shaders/")

find_package(OpenGL REQUIRED)
find_package(glbinding REQUIRED)

if(MSVC)
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/PlatformWindowsMSVC.cmake)
    set(CMAKE_CXX_FLAGS -EHa)
    set(GLBINDING_LIBRARIES ${LIB_DIR}/glbindingd.lib)
    set(GLFW_LIBRARIES ${LIB_DIR}/glfw3.lib)
elseif(WIN32 AND CMAKE_COMPILER_IS_GNUCXX)
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/PlatformWindowsGCC.cmake)
elseif(APPLE)
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/PlatformMacOS.cmake)
elseif(UNIX)
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/PlatformLinux.cmake)
    find_package(PkgConfig REQUIRED)
    pkg_search_module(GLFW REQUIRED glfw3)
    set(GLFW_LIBRARIES ${GLFW_STATIC_LIBRARIES})
else()
    message(WARNING "Unsupported platform/compiler combination")
endif()

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
    ${GLFW_LIBRARIES}
    TacoGL
)
