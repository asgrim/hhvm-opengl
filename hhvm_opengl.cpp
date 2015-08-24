#include "hphp/runtime/ext/extension.h"
#include "hphp/runtime/vm/native-data.h"

#include <stdio.h>
#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>
using namespace glm;

#include "lib/shader.hpp"
#include "lib/WriteBmp.h"

namespace HPHP {

    const StaticString
            s_OpenGL("OpenGL");

    class OpenGL {
    public:
        OpenGL() {}

        ~OpenGL() { sweep(); }
        void sweep() {
        }

        GLuint programID;
        GLuint vertexPosition_modelspaceID;
        GLuint vertexBuffer;
    };

    void HHVM_METHOD(OpenGL, __construct) {
        auto data = Native::data<OpenGL>(this_);

        if (!glfwInit()) {
            throw Object(SystemLib::AllocExceptionObject("Unable to init GLFW"));
        }

        glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
        glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 2);
        glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 1);
        glfwOpenWindow(800, 600, 1, 1, 1, 0, 32, 0, GLFW_WINDOW);
        glewInit();
        glfwSetWindowTitle( "Hello world" );
        glfwEnable( GLFW_STICKY_KEYS );
        glClearColor(0.0f, 0.0f, 0.9f, 0.0f);

        data->programID = LoadShaders( "shaders/SimpleVertexShader.vertexshader", "shaders/SimpleFragmentShader.fragmentshader" );
        data->vertexPosition_modelspaceID = glGetAttribLocation(data->programID, "vertexPosition_modelspace");

        static const GLfloat g_vertex_buffer_data[] = {
                -1.0f, -1.0f, 0.0f,
                1.0f, -1.0f, 0.0f,
                0.0f,  1.0f, 0.0f,
        };

        glGenBuffers(1, &data->vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, data->vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    }

    void HHVM_METHOD(OpenGL, render) {
        auto data = Native::data<OpenGL>(this_);

        glClear( GL_COLOR_BUFFER_BIT );
        glUseProgram(data->programID);
        glEnableVertexAttribArray(data->vertexPosition_modelspaceID);
        glBindBuffer(GL_ARRAY_BUFFER, data->vertexBuffer);
        glVertexAttribPointer(
                data->vertexPosition_modelspaceID, // The attribute we want to configure
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0            // array buffer offset
        );

        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle

        glDisableVertexAttribArray(data->vertexPosition_modelspaceID);

        glfwSwapBuffers();
    }

    String HHVM_METHOD(OpenGL, getBackBufferContent) {
        GLubyte *data = (GLubyte*)malloc(2 * 800 * 600 * sizeof(GLubyte));
        glReadBuffer(GL_BACK);
        glReadPixels(0, 0, 800, 600, GL_RGB, GL_UNSIGNED_BYTE, data);

        String filename = String("/tmp/lol.bmp");
        WriteBMP(filename.c_str(), 800, 600, data);

        return filename;
    }

    void HHVM_METHOD(OpenGL, close) {
        glfwTerminate();
    }

    class HHVMOpenGLExtension : public Extension {
    public:
        HHVMOpenGLExtension() : Extension("hhvm_opengl", "1.0") {}
        virtual void moduleInit() {

            HHVM_ME(OpenGL, __construct);
            HHVM_ME(OpenGL, render);
            HHVM_ME(OpenGL, getBackBufferContent);
            HHVM_ME(OpenGL, close);
            Native::registerNativeDataInfo<OpenGL>(s_OpenGL.get());

            loadSystemlib();
        }

    } s_hhvm_opengl_extension;

    HHVM_GET_MODULE(hhvm_opengl)

} // namespace HPHP