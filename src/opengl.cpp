#include <stdio.h>

#include "hphp/runtime/ext/extension.h"
#include "hphp/runtime/vm/native-data.h"
#include "hphp/runtime/vm/class.h"
#include "hphp/runtime/base/execution-context.h"
#include "../hhvm_opengl.h"

#include <GL/glew.h>
#include <GL/glfw.h>

#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "../lib/shader.hpp"
#include "../lib/WriteBmp.h"

namespace HPHP {

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

const StaticString s_OpenGL("OpenGL");

class OpenGL {
public:
    OpenGL() { }

    ~OpenGL() { sweep(); }

    void sweep() {
    }

    int width;
    int height;

    GLuint programID;
    GLuint vertexPosition_modelspaceID;
    GLuint vertexBuffer;
    GLsizei vertexBufferSize;
    GLuint colourBuffer;
    GLuint MatrixID;
    glm::mat4 MVP;
    glm::mat4 Model;
    glm::mat4 View;
    glm::mat4 Projection;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void HHVM_METHOD(OpenGL, __construct, int width, int height) {
    auto data = Native::data<OpenGL>(this_);

    data->width = width;
    data->height = height;

    if (!glfwInit()) {
        throw Object(SystemLib::AllocExceptionObject("Unable to init GLFW"));
    }

    glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 2);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 1);

    if (!glfwOpenWindow(data->width, data->height, 0, 0, 0, 0, 32, 0, GLFW_WINDOW)) {
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

    data->programID = LoadShaders("shaders/SimpleVertexShader.vertexshader",
                                  "shaders/SimpleFragmentShader.fragmentshader");
    data->MatrixID = glGetUniformLocation(data->programID, "MVP");
    data->vertexPosition_modelspaceID = glGetAttribLocation(data->programID, "vertexPosition_modelspace");
}

void HHVM_METHOD(OpenGL, setCameraSettings, const Array& cameraSettings) {
    auto data = Native::data<OpenGL>(this_);

    ArrayData *camSettings = cameraSettings.get();

    data->Projection = glm::perspective(
            (float)camSettings->getValue(0).toDouble(),
            (float)camSettings->getValue(1).toDouble() / (float)camSettings->getValue(2).toDouble(),
            (float)camSettings->getValue(3).toDouble(),
            (float)camSettings->getValue(4).toDouble()
    );

    data->View = glm::lookAt(
            glm::vec3((float)camSettings->getValue(5).toDouble(), (float)camSettings->getValue(6).toDouble(), (float)camSettings->getValue(7).toDouble()), // Camera is at (4,3,3), in World Space
            glm::vec3((float)camSettings->getValue(8).toDouble(), (float)camSettings->getValue(9).toDouble(), (float)camSettings->getValue(10).toDouble()), // and looks at the origin
            glm::vec3(0, (float)camSettings->getValue(11).toDouble(), 0)  // Head is up (set to 0,-1,0 to look upside-down)
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

void HHVM_METHOD(OpenGL, setVertexBuffer, const Array& vertexBuffer) {
    auto data = Native::data<OpenGL>(this_);

    ArrayData *verticesArray = vertexBuffer.get();

    GLfloat  g_vertex_buffer_data[verticesArray->getSize()];
    size_t len = verticesArray->size();
    for (int i = 0; i < len; i++) {
        g_vertex_buffer_data[i] = (float)verticesArray->getValue(i).toDouble();
    }

    data->vertexBufferSize = sizeof(g_vertex_buffer_data) / sizeof(GLfloat) / 3;

    glGenBuffers(1, &data->vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, data->vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
}

void HHVM_METHOD(OpenGL, setColourBuffer, const Array& colourBuffer) {
    auto data = Native::data<OpenGL>(this_);

    ArrayData *coloursArray = colourBuffer.get();

    GLfloat  g_color_buffer_data[coloursArray->getSize()];
    size_t len = coloursArray->size();
    for (int i = 0; i < len; i++) {
        g_color_buffer_data[i] = (float) coloursArray->getValue(i).toDouble();
    }

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
            (void *) 0            // array buffer offset
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
            (void *) 0            // array buffer offset
    );

    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, data->vertexBufferSize);

    glDisableVertexAttribArray(data->vertexPosition_modelspaceID);

    glfwSwapBuffers();
}

String HHVM_METHOD(OpenGL, getBackBufferContent) {
    auto data = Native::data<OpenGL>(this_);

    GLubyte *pixels = (GLubyte *) malloc(2 * data->width * data->height * sizeof(GLubyte) * 100);
    glReadBuffer(GL_FRONT);
    glReadPixels(0, 0, data->width, data->height, GL_RGB, GL_UNSIGNED_BYTE, pixels);

    // @todo get rid of the writing to disk and return BMP directly
    String filename = String("/tmp/lol.bmp");
    WriteBMP(filename.c_str(), data->width, data->height, pixels);

    free(pixels);

    return filename;
}

void HHVM_METHOD(OpenGL, close) {
    auto data = Native::data<OpenGL>(this_);

    glDeleteBuffers(1, &data->vertexBuffer);
    glDeleteProgram(data->programID);
    glfwTerminate();
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void HHVMOpenGLExtension::initOpenGLClass() {
    HHVM_ME(OpenGL, __construct);
    HHVM_ME(OpenGL, render);
    HHVM_ME(OpenGL, setCameraSettings);
    HHVM_ME(OpenGL, setBackgroundColour);
    HHVM_ME(OpenGL, setVertexBuffer);
    HHVM_ME(OpenGL, setColourBuffer);
    HHVM_ME(OpenGL, getBackBufferContent);
    HHVM_ME(OpenGL, close);
    Native::registerNativeDataInfo<OpenGL>(s_OpenGL.get());
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

} // namespace HPHP
