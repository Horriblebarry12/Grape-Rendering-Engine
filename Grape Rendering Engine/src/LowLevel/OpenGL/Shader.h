#pragma once
#include <string>
#include "glm/gtc/matrix_transform.hpp"
#include "OpenGLPCH.h"

struct GP_API ShaderProgranSource
{
	std::string VertexSource;
	std::string FragmentSource;
};


class GP_API Shader
{
public:
	Shader(const std::string& filename = "res/shaders/DefaultShader.shader");
	~Shader();

	void Bind() const;
	void Unbind() const;

	// Uniforms
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniform1f(const std::string& name, float v0);
	void SetUniform1i(const std::string& name, int v0);
	void SetUniformMat4f(const std::string& name, const glm::mat4& mat);
private:
	
	std::string p_Filepath;
	unsigned int p_RendererID;

	ShaderProgranSource PlaseShader(const std::string& filepath);
	unsigned int CompileShader(const std::string& source, unsigned int type);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	int GetUniformLocation(const std::string& name);
};

