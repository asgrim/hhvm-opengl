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
        GLuint colourBuffer;
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

        if(!glfwOpenWindow(data->width, data->height, 0, 0, 0, 0, 32, 0, GLFW_WINDOW)) {
            glfwTerminate();
            throw Object(SystemLib::AllocExceptionObject("Failed to open GLFW window."));
        }

        if (glewInit() != GLEW_OK) {
            glfwTerminate();
            throw Object(SystemLib::AllocExceptionObject("Unable to init GLEW"));
        }

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

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
    }

    void HHVM_METHOD(OpenGL, setBackgroundColour, double r, double g, double b, double a) {
        glClearColor(
            (GLclampf) r,
            (GLclampf) g,
            (GLclampf) b,
            (GLclampf) a
        );
    }

    void HHVM_METHOD(OpenGL, setVertexBuffer) {
        auto data = Native::data<OpenGL>(this_);

        static const GLfloat g_vertex_buffer_data[] = {
                -1.0f,-1.0f,-1.0f, // triangle 1 : begin
                -1.0f,-1.0f, 1.0f,
                -1.0f, 1.0f, 1.0f, // triangle 1 : end
                1.0f, 1.0f,-1.0f, // triangle 2 : begin
                -1.0f,-1.0f,-1.0f,
                -1.0f, 1.0f,-1.0f, // triangle 2 : end
                1.0f,-1.0f, 1.0f,
                -1.0f,-1.0f,-1.0f,
                1.0f,-1.0f,-1.0f,
                1.0f, 1.0f,-1.0f,
                1.0f,-1.0f,-1.0f,
                -1.0f,-1.0f,-1.0f,
                -1.0f,-1.0f,-1.0f,
                -1.0f, 1.0f, 1.0f,
                -1.0f, 1.0f,-1.0f,
                1.0f,-1.0f, 1.0f,
                -1.0f,-1.0f, 1.0f,
                -1.0f,-1.0f,-1.0f,
                -1.0f, 1.0f, 1.0f,
                -1.0f,-1.0f, 1.0f,
                1.0f,-1.0f, 1.0f,
                1.0f, 1.0f, 1.0f,
                1.0f,-1.0f,-1.0f,
                1.0f, 1.0f,-1.0f,
                1.0f,-1.0f,-1.0f,
                1.0f, 1.0f, 1.0f,
                1.0f,-1.0f, 1.0f,
                1.0f, 1.0f, 1.0f,
                1.0f, 1.0f,-1.0f,
                -1.0f, 1.0f,-1.0f,
                1.0f, 1.0f, 1.0f,
                -1.0f, 1.0f,-1.0f,
                -1.0f, 1.0f, 1.0f,
                1.0f, 1.0f, 1.0f,
                -1.0f, 1.0f, 1.0f,
                1.0f,-1.0f, 1.0f
        };

        glGenBuffers(1, &data->vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, data->vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    }

    void HHVM_METHOD(OpenGL, setColourBuffer) {
        auto data = Native::data<OpenGL>(this_);

        static const GLfloat g_color_buffer_data[] = {
                0.583f,  0.771f,  0.014f,
                0.609f,  0.115f,  0.436f,
                0.327f,  0.483f,  0.844f,
                0.822f,  0.569f,  0.201f,
                0.435f,  0.602f,  0.223f,
                0.310f,  0.747f,  0.185f,
                0.597f,  0.770f,  0.761f,
                0.559f,  0.436f,  0.730f,
                0.359f,  0.583f,  0.152f,
                0.483f,  0.596f,  0.789f,
                0.559f,  0.861f,  0.639f,
                0.195f,  0.548f,  0.859f,
                0.014f,  0.184f,  0.576f,
                0.771f,  0.328f,  0.970f,
                0.406f,  0.615f,  0.116f,
                0.676f,  0.977f,  0.133f,
                0.971f,  0.572f,  0.833f,
                0.140f,  0.616f,  0.489f,
                0.997f,  0.513f,  0.064f,
                0.945f,  0.719f,  0.592f,
                0.543f,  0.021f,  0.978f,
                0.279f,  0.317f,  0.505f,
                0.167f,  0.620f,  0.077f,
                0.347f,  0.857f,  0.137f,
                0.055f,  0.953f,  0.042f,
                0.714f,  0.505f,  0.345f,
                0.783f,  0.290f,  0.734f,
                0.722f,  0.645f,  0.174f,
                0.302f,  0.455f,  0.848f,
                0.225f,  0.587f,  0.040f,
                0.517f,  0.713f,  0.338f,
                0.053f,  0.959f,  0.120f,
                0.393f,  0.621f,  0.362f,
                0.673f,  0.211f,  0.457f,
                0.820f,  0.883f,  0.371f,
                0.982f,  0.099f,  0.879f
        };

        glGenBuffers(1, &data->colourBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, data->colourBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
    }

    void HHVM_METHOD(OpenGL, render) {
        auto data = Native::data<OpenGL>(this_);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(data->programID);

        glUniformMatrix4fv(data->MatrixID, 1, GL_FALSE, &data->MVP[0][0]);

        // Vertex buffer
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

        // Colour buffer
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, data->colourBuffer);
        glVertexAttribPointer(
                1, // The attribute we want to configure
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0            // array buffer offset
        );

        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, 12*3);

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
            HHVM_ME(OpenGL, setVertexBuffer);
            HHVM_ME(OpenGL, setColourBuffer);
            HHVM_ME(OpenGL, getBackBufferContent);
            HHVM_ME(OpenGL, close);
            Native::registerNativeDataInfo<OpenGL>(s_OpenGL.get());

            loadSystemlib();
        }

    } s_hhvm_opengl_extension;

    HHVM_GET_MODULE(hhvm_opengl)

} // namespace HPHP