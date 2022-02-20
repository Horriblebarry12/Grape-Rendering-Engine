#include "OpenGLRenderer.h"

OpenGLRenderer::OpenGLRenderer()
{

}

void OpenGLRenderer::Init()
{

	/* Initialize the library */
	if (!glfwInit())
	{
		//TODO: LogCritical("Glfw failed to initalize!");
		return;
	}

	window = glfwCreateWindow(1920, 1080, "ThisIsAWindow", NULL, NULL);
	if (!window)
	{
		//TODO: LogCritical("Window failed to create!");
		glfwTerminate();
		return;
	}


	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
	{
		//LogCritical("Glew failed to initalize!");
		return;
	}
	glfwSwapInterval(1);

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback( &OpenGLRenderer::GLDebugMessageCallback, nullptr);
}

void OpenGLRenderer::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderer::DrawIndexed(OpenGLVertexBuffer& vb, DrawType drawType) const
{
	vb.BindRendering();

	glDrawElements(GL_TRIANGLES, vb.GetIndexCount(), GL_UNSIGNED_INT, nullptr);
}

void OpenGLRenderer::GLDebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* msg, const void* data)
{
    std::string _source;
    std::string _type;
    std::string _severity;

    switch (source)
    {
    case GL_DEBUG_SOURCE_API:
        _source = "API";
        break;

    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        _source = "WINDOW SYSTEM";
        break;

    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        _source = "SHADER COMPILER";
        break;

    case GL_DEBUG_SOURCE_THIRD_PARTY:
        _source = "THIRD PARTY";
        break;

    case GL_DEBUG_SOURCE_APPLICATION:
        _source = "APPLICATION";
        break;

    case GL_DEBUG_SOURCE_OTHER:
        _source = "UNKNOWN";
        break;

    default:
        _source = "UNKNOWN";
        break;
    }

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:
        _type = "ERROR";
        break;

    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        _type = "DEPRECATED BEHAVIOR";
        break;

    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        _type = "UDEFINED BEHAVIOR";
        break;

    case GL_DEBUG_TYPE_PORTABILITY:
        _type = "PORTABILITY";
        break;

    case GL_DEBUG_TYPE_PERFORMANCE:
        _type = "PERFORMANCE";
        break;

    case GL_DEBUG_TYPE_OTHER:
        _type = "OTHER";
        break;

    case GL_DEBUG_TYPE_MARKER:
        _type = "MARKER";
        break;

    default:
        _type = "UNKNOWN";
        break;
    }

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:
        _severity = "HIGH";
        break;

    case GL_DEBUG_SEVERITY_MEDIUM:
        _severity = "MEDIUM";
        break;

    case GL_DEBUG_SEVERITY_LOW:
        _severity = "LOW";
        break;

    case GL_DEBUG_SEVERITY_NOTIFICATION:
        _severity = "NOTIFICATION";
        break;

    default:
        _severity = "UNKNOWN";
        break;
    }

    //TODO: Log this properly
    std::cout << "[ Log Source: RENDERER ] [ Type: " << _type << " ] [ Severity: " << _severity << " ] [ Source: " << _source << " ] " << "Error Code:" << id << " Msg: " << msg << " | " << data << "\n";

    //if( _severity != "NOTIFICATION" )
        //__debugbreak();
}
