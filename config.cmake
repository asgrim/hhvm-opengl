project(hhvm_opengl)

# cmake modules
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

# Locate packages
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLFW REQUIRED)

# Definitions of things
add_definitions(${GLFW_CFLAGS_OTHER})
add_definitions(
    -lGL
    -lGLU
    -lGLEW
    -lglfw
)

HHVM_EXTENSION(hhvm_opengl hhvm_opengl.cpp lib/shader.cpp lib/WriteBmp.cpp)
HHVM_SYSTEMLIB(hhvm_opengl ext_hhvm_opengl.php)

target_link_libraries(hhvm_opengl ${OPENGL_LIBRARY} ${GLFW_LIBRARY} ${GLEW_LIBRARY})
