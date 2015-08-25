#include "hphp/runtime/ext/extension.h"
#include "hhvm_opengl.h"

namespace HPHP {

HHVMOpenGLExtension::HHVMOpenGLExtension() :
    Extension("hhvm_opengl", "1.0") {}

void HHVMOpenGLExtension::moduleInit() {
    initOpenGLClass();
    initOpenGLCoordinateClass();
    loadSystemlib();
}

HHVMOpenGLExtension s_hhvm_opengl_extension;
HHVM_GET_MODULE(hhvm_opengl);

} // namespace HPHP