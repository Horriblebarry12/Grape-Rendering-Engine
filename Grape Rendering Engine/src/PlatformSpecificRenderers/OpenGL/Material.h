#pragma once
#include <iostream>

#include "GL/glew.h"
#include "../GrapePCH.h"
#include "glm/mat4x4.hpp" // glm::mat4
#include "../LowLevel/OpenGL/Debug.h"
struct Material
{
public:

	Material(std::string filename) : Filename(filename)
	{
		ShaderProgranSource source = PlaseShader(filename);

		RendererID = CreateShader(source.VertexSource, source.FragmentSource);
	}

	Material(const Material& copy)
	{
		RendererID = copy.RendererID;
		Filename = copy.Filename;
	}

	// TODO: Default constructor for material class
	Material()
	{

	}

	~Material()
	{
		glDeleteProgram(RendererID);
	}

#pragma region Uniforms

	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniform1f(const std::string& name, float v0);
	void SetUniform1i(const std::string& name, int v0);
	void SetUniformMat4f(const std::string& name, const glm::mat4& mat);

#pragma endregion



	inline void Bind() const { glUseProgram(RendererID); };

	inline void UnBind() const { glUseProgram(0); };

	inline UINT32 getRenderID() { return RendererID; };

	inline std::string geFilename() { return Filename; };

private:
#pragma region Shader_Setup

	struct ShaderProgranSource
	{
		std::string VertexSource;
		std::string FragmentSource;
	};

	UINT32 CompileShader(const std::string& source, unsigned int type)
	{
		UINT32 id = glCreateShader(type);

		const char* src = source.c_str();

		glShaderSource(id, 1, &src, NULL);

		glCompileShader(id);

		int result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);

		if (result == GL_FALSE)
		{
			int length;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

			char* message = (char*)_malloca(length * sizeof(char));

			glGetShaderInfoLog(id, length, &length, message);


			std::cout << message;
		}

		return id;
	}

	Material::ShaderProgranSource PlaseShader(const std::string& filepath)
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

	UINT32 CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
	{
		UINT32 vs = CompileShader(vertexShader, GL_VERTEX_SHADER);
		UINT32 fs = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);
		UINT32 program = glCreateProgram();

		glAttachShader(program, vs);
		glAttachShader(program, fs);
		glLinkProgram(program);
		glValidateProgram(program);

		return program;
	}

#pragma endregion

	int GetUniformLocation(const std::string& name);

	UINT32 RendererID;

	std::string Filename;

	std::map<std::string, int> UniformLocations;
};