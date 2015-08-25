#include "hphp/runtime/ext/extension.h"
#include "hphp/runtime/vm/native-data.h"
#include "../hhvm_opengl.h"

namespace HPHP {

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

const StaticString s_OpenGLCoordinate("OpenGLCoordinate");

class OpenGLCoordinate {
public:
    OpenGLCoordinate() { }
    ~OpenGLCoordinate() { sweep(); }

    void sweep() {
    }

    int x;
    int y;
    int z;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////


void HHVM_METHOD(OpenGLCoordinate, __construct, int x, int y, int z) {
    auto data = Native::data<OpenGLCoordinate>(this_);
    data->x = x;
    data->y = y;
    data->z = z;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void HHVMOpenGLExtension::initOpenGLCoordinateClass() {
    HHVM_ME(OpenGLCoordinate, __construct);
    Native::registerNativeDataInfo<OpenGLCoordinate>(s_OpenGLCoordinate.get());
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

} // namespace HPHP