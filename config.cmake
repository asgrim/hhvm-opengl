ADD_DEFINITIONS(
    -lGL
)

FIND_PACKAGE(OpenGL)

HHVM_EXTENSION(hhvm_opengl hhvm_opengl.cpp)
HHVM_SYSTEMLIB(hhvm_opengl ext_hhvm_opengl.php)

target_link_libraries(hhvm_opengl ${OPENGL_LIBRARY})