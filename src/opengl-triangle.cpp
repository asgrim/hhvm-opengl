#include "hphp/runtime/ext/extension.h"
#include "hphp/runtime/vm/native-data.h"
#include "../hhvm_opengl.h"
#include "opengl-vertex.h"

namespace HPHP {

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

const StaticString s_OpenGLTriangle("OpenGLTriangle");

class OpenGLTriangle {
public:
    OpenGLTriangle() { }
    ~OpenGLTriangle() { sweep(); }

    void sweep() {
    }

    Object a;
    Object b;
    Object c;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void HHVM_METHOD(OpenGLTriangle, __construct, const Object& a, const Object& b, const Object& c) {
    auto data = Native::data<OpenGLTriangle>(this_);

    data->a = a;
    data->b = b;
    data->c = c;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void HHVMOpenGLExtension::initOpenGLTriangleClass() {
    HHVM_ME(OpenGLTriangle, __construct);
    Native::registerNativeDataInfo<OpenGLTriangle>(s_OpenGLTriangle.get());
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

} // namespace HPHP