#include "hphp/runtime/ext/extension.h"
#include "hphp/runtime/vm/native-data.h"

#include <stdio.h>
#include <GL/glew.h>
#include <GL/glfw.h>

#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
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

        int width;
        int height;

        GLuint programID;
        GLuint vertexPosition_modelspaceID;
        GLuint vertexBuffer;
        GLuint MatrixID;
        glm::mat4 MVP;
        glm::mat4 Model;
        glm::mat4 View;
        glm::mat4 Projection;
    };

    void HHVM_METHOD(OpenGL, __construct) {
        auto data = Native::data<OpenGL>(this_);

        data->width = 1024;
        data->height = 768;

        if (!glfwInit()) {
            throw Object(SystemLib::AllocExceptionObject("Unable to init GLFW"));
        }

        glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
        glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 2);
        glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 1);
        glfwOpenWindow(data->width, data->height, 1, 1, 1, 0, 32, 0, GLFW_WINDOW);
        glewInit();
        glfwSetWindowTitle( "Hello world" );
        glfwEnable( GLFW_STICKY_KEYS );
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

        data->programID = LoadShaders( "shaders/SimpleVertexShader.vertexshader", "shaders/SimpleFragmentShader.fragmentshader" );
        data->MatrixID = glGetUniformLocation(data->programID, "MVP");
        data->vertexPosition_modelspaceID = glGetAttribLocation(data->programID, "vertexPosition_modelspace");

        data->Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
        data->View = glm::lookAt(
            glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
            glm::vec3(0,0,0), // and looks at the origin
            glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
        );
        data->Model = glm::mat4(1.0f);
        data->MVP = data->Projection * data->View * data->Model;

        static const GLfloat g_vertex_buffer_data[] = {
                -1.0f, -1.0f, 0.0f,
                1.0f, -1.0f, 0.0f,
                0.0f,  1.0f, 0.0f,
        };

        glGenBuffers(1, &data->vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, data->vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    }

    void HHVM_METHOD(OpenGL, setBackgroundColour, double r, double g, double b, double a) {
        glClearColor(
            (GLclampf) r,
            (GLclampf) g,
            (GLclampf) b,
            (GLclampf) a
        );
    }

    void HHVM_METHOD(OpenGL, render) {
        auto data = Native::data<OpenGL>(this_);

        glClear( GL_COLOR_BUFFER_BIT );
        glUseProgram(data->programID);

        glUniformMatrix4fv(data->MatrixID, 1, GL_FALSE, &data->MVP[0][0]);

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
        auto data = Native::data<OpenGL>(this_);

        GLubyte *pixels = (GLubyte*)malloc(2 * data->width * data->height * sizeof(GLubyte));
        glReadBuffer(GL_BACK);
        glReadPixels(0, 0, data->width, data->height, GL_RGB, GL_UNSIGNED_BYTE, pixels);

        // @todo get rid of the writing to disk and return BMP directly
        String filename = String("/tmp/lol.bmp");
        WriteBMP(filename.c_str(), data->width, data->height, pixels);

        return filename;
    }

    void HHVM_METHOD(OpenGL, close) {
        auto data = Native::data<OpenGL>(this_);

        glDeleteBuffers(1, &data->vertexBuffer);
        glDeleteProgram(data->programID);
        glfwTerminate();
    }

    class HHVMOpenGLExtension : public Extension {
    public:
        HHVMOpenGLExtension() : Extension("hhvm_opengl", "1.0") {}
        virtual void moduleInit() {

            HHVM_ME(OpenGL, __construct);
            HHVM_ME(OpenGL, render);
            HHVM_ME(OpenGL, setBackgroundColour);
            HHVM_ME(OpenGL, getBackBufferContent);
            HHVM_ME(OpenGL, close);
            Native::registerNativeDataInfo<OpenGL>(s_OpenGL.get());

            loadSystemlib();
        }

    } s_hhvm_opengl_extension;

    HHVM_GET_MODULE(hhvm_opengl)

} // namespace HPHP