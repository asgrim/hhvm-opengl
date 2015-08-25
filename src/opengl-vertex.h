//
// Created by james on 25/08/15.
//

#ifndef HHVM_OPENGL_OPENGL_VERTEX_H
#define HHVM_OPENGL_OPENGL_VERTEX_H
namespace HPHP {

class OpenGLVertex {
public:
    OpenGLVertex();
    ~OpenGLVertex();

    void sweep();

    int x;
    int y;
    int z;
};

} // namespace HPHP
#endif //HHVM_OPENGL_OPENGL_VERTEX_H
