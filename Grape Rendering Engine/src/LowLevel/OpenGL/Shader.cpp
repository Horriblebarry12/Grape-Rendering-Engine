#include "Shader.h"
#include "Debug.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string& filename)  : p_Filepath(filename), p_RendererID(0)
{
	ShaderProgranSource source = PlaseShader(filename);

	p_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(p_RendererID));
}

void Shader::Bind() const
{
	GLCall(glUseProgram(p_RendererID));
}

void Shader::Unbind() const
{
	GLCall(glUseProgram(0));
}


ShaderProgranSource Shader::PlaseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::stringstream ss[2];

	std::string line;

	ShaderType type = ShaderType::NONE;

	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			ss[(int)type] << line << "\n";
		}

	}

	return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(const std::string& source, unsigned int type)
{
	GLCall(unsigned int id = glCreateShader(type));

	const char* src = source.c_str();

	GLCall(glShaderSource(id, 1, &src, NULL));

	GLCall(glCompileShader(id));
	
	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

	if (result == GL_FALSE)
	{
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));

		char* message = (char*)_malloca(length * sizeof(char));

		GLCall(glGetShaderInfoLog(id, length, &length, message));


		std::cout << message << "\n";

	}

	return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	GLCall(unsigned int program = glCreateProgram());
	unsigned int vs = CompileShader(vertexShader, GL_VERTEX_SHADER);
	unsigned int fs = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);

	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	return program;
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniform1f(const std::string& name, float v0)
{
	GLCall(glUniform1f(GetUniformLocation(name), v0));
}

void Shader::SetUniform1i(const std::string& name, int v0)
{
	GLCall(glUniform1i(GetUniformLocation(name), v0));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& mat)
{
	GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]));
}

int Shader::GetUniformLocation(const std::string& name)
{
	GLCall(int location = glGetUniformLocation(p_RendererID, name.c_str()));
	if (location == -1) 
	{
		LogError("Couldn't find uniform \"{}\"", name);
	}
	return location;
}
