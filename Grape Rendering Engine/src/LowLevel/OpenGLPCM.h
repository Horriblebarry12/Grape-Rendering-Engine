#pragma once

#if USE_OPENGL

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "OpenGL\Debug.h"
#include "OpenGL\VertexBuffer.h"
#include "OpenGL\IndexBuffer.h"
#include "OpenGL\VertexArray.h"
#include "OpenGL\BufferLayout.h"
#include "OpenGL\Shader.h"
#include "OpenGL\Renderer.h"
#include "OpenGL\Texture.h"
#include "ImGUI\imgui.h"
#include "ImGUI\imgui_impl_opengl3.h"
#include "ImGUI\imgui_impl_glfw.h"


#endif // USE_OPENGL