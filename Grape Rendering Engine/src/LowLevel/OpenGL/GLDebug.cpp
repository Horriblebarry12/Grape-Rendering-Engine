#include <iostream>
#include "GLDebug.h"
#include "GLFW/glfw3.h"

void OpenGLDebug::GLClearError()
{
    while (glGetError())
    {

    }
}

bool OpenGLDebug::GLLogCall(const char* funtion, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        LogError("[OpenGL] {} \n At Funtion: {} \n At Line: {} At File: {} \n", error, funtion, line, file);

        return false;
    }
    return true;
}

