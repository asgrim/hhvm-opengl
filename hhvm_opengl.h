//
// Created by james on 25/08/15.
//

#ifndef HHVM_OPENGL_HHVM_OPENGL_H
#define HHVM_OPENGL_HHVM_OPENGL_H

namespace HPHP {
    class HHVMOpenGLExtension : public Extension {
    public:
        HHVMOpenGLExtension();

        virtual void moduleInit();

    private:
        void initOpenGLClass();
        void initOpenGLCoordinateClass();
    };
}

#endif //HHVM_OPENGL_HHVM_OPENGL_H
