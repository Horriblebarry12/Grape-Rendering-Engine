#include "OpenGLRenderer.h"
#include "../Debug.h"
#include "../Mesh.h"
#include "../Model.h"
#include "stb_image/stb_image.h"

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

void OpenGLRenderer::DrawMesh(Mesh& mesh)
{
    mesh.mat.Bind();
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    for (unsigned int i = 0; i < mesh.textures.size(); i++)
    {
        if (!mesh.textures[i].isSetup)
        {
            SetupTexture(mesh.textures[i]);
        }
        glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        std::string number;
        std::string name = mesh.textures[i].type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++);

        mesh.mat. SetVar1f(("material." + name + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, mesh.textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);

    // draw mesh
    glBindVertexArray(mesh.VAO);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    mesh.mat.UnBind();

}

void OpenGLRenderer::DrawModel(Model& model)
{
    for (UINT64 i = 0; i < model.meshes.size(); i++)
    {
        DrawMesh(model.meshes[i]);
    }
}

void OpenGLRenderer::SetupMesh(Mesh& mesh)
{
    glGenVertexArrays(1, &mesh.VAO);
    glGenBuffers(1, &mesh.VBO);
    glGenBuffers(1, &mesh.EBO);

    glBindVertexArray(mesh.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);

    glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(Vertex), &mesh.vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(unsigned int),
        &mesh.indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
}

void OpenGLRenderer::SetupModel(Model& model)
{
    for (UINT64 i = 0; i < model.meshes.size(); i++)
    {
        SetupMesh(model.meshes[i]);
    }
}

UINT32 OpenGLRenderer::SetupTexture(std::string path)
{
    std::string filename = std::string(path);

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

bool OpenGLRenderer::SetupTexture(Texture tex)
{
    if (tex.isSetup)
    {
        return false;
    }

    std::string filename = std::string(tex.path);

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(tex.path.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << tex.path << std::endl;
        stbi_image_free(data);
    }

    tex.id = textureID;

    return true;
}

void OpenGLRenderer::GLDebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* msg, const void* data)
{
    std::string logSource;
    std::string logType;

    switch (source)
    {
    case GL_DEBUG_SOURCE_API:
        logSource = "API";
        break;

    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        logSource = "WINDOW SYSTEM";
        break;

    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        logSource = "SHADER COMPILER";
        break;

    case GL_DEBUG_SOURCE_THIRD_PARTY:
        logSource = "THIRD PARTY";
        break;

    case GL_DEBUG_SOURCE_APPLICATION:
        logSource = "APPLICATION";
        break;

    case GL_DEBUG_SOURCE_OTHER:
        logSource = "UNKNOWN";
        break;

    default:
        logSource = "UNKNOWN";
        break;
    }

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:
        logType = "ERROR";
        break;

    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        logType = "DEPRECATED BEHAVIOR";
        break;

    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        logType = "UDEFINED BEHAVIOR";
        break;

    case GL_DEBUG_TYPE_PORTABILITY:
        logType = "PORTABILITY";
        break;

    case GL_DEBUG_TYPE_PERFORMANCE:
        logType = "PERFORMANCE";
        break;

    case GL_DEBUG_TYPE_OTHER:
        logType = "OTHER";
        break;

    case GL_DEBUG_TYPE_MARKER:
        logType = "MARKER";
        break;

    default:
        logType = "UNKNOWN";
        break;
    }

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:
        LogError("[OPENGL] [Type: {}] [Source: {}] Code: {} | Msg: {} | Data: {}", logType, logSource, id, msg, data);
        // __debugbreak();
        break;

    case GL_DEBUG_SEVERITY_MEDIUM:
        LogWarning("[OPENGL] [Type: {}] [Source: {}] Code: {} | Msg: {} | Data: {}", logType, logSource, id, msg, data);
        break;

    case GL_DEBUG_SEVERITY_LOW:
        LogInfo("[OPENGL] [Type: {}] [Source: {}] Code: {} | Msg: {} | Data: {}", logType, logSource, id, msg, data);
        break;

    case GL_DEBUG_SEVERITY_NOTIFICATION:
        LogDebug("[OPENGL] [Type: {}] [Source: {}] Code: {} | Msg: {} | Data: {}", logType, logSource, id, msg, data);
        break;

    default:
        LogDebug("[OPENGL] [Type: {}] [Source: {}] Code: {} | Msg: {} | Data: {}", logType, logSource, id, msg, data);
        break;
    }
}
