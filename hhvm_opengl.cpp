#include "hphp/runtime/ext/extension.h"
#include "hphp/runtime/vm/native-data.h"

namespace HPHP {

    const StaticString
            s_OpenGL("OpenGL");

    class OpenGL {
    public:
        OpenGL() {}

        ~OpenGL() { sweep(); }
        void sweep() {
        }
    };

    void HHVM_METHOD(OpenGL, __construct) {
        auto data = Native::data<OpenGL>(this_);
    }

    class HHVMOpenGLExtension : public Extension {
    public:
        HHVMOpenGLExtension() : Extension("hhvm_opengl", "1.0") {}
        virtual void moduleInit() {

            HHVM_ME(OpenGL, __construct);
            Native::registerNativeDataInfo<OpenGL>(s_OpenGL.get());

            loadSystemlib();
        }

    } s_hhvm_opengl_extension;

    HHVM_GET_MODULE(hhvm_opengl)

} // namespace HPHP