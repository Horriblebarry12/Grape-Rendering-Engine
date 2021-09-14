#include <iostream>
#include "Debug.h"
#include "GLFW/glfw3.h"

void Debug::GLClearError()
{
    while (glGetError())
    {

    }
}

bool Debug::GLLogCall(const char* funtion, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL] [Error]" << error << "\n";
        std::cout << "At Funtion: " << funtion << " At Line: " << line << " At File: " << file;
        return false;
    }
    return true;
}

void Debug::LogInfo(std::string msg)
{
    std::cout << msg << "\n";
}
