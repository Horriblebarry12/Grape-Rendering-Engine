#pragma once
#include <iostream>

#include "GL/glew.h"
#include "GrapePCH.h"
#include "Debug.h"
#include "Renderers/Material.h"

class GP_API GLMaterial : public Material
{
public:

	GLMaterial(std::string filename)
	{
		Filename = filename;

		ShaderProgranSource source = PlaseShader(filename);

		RendererID = CreateShader(source.VertexSource, source.FragmentSource);
	}

	GLMaterial(const GLMaterial& copy)
	{
		RendererID = copy.RendererID;
		Filename = copy.Filename;
	}

	// TODO: Default constructor for material class
	GLMaterial()
	{
		
	}

	~GLMaterial()
	{
		glDeleteProgram(RendererID);
	}

#pragma region Uniforms

	void SetVar4f(const std::string& name, float v0, float v1, float v2, float v3) override;
	void SetVar4f(const std::string& name, glm::vec4 vec4) override;
	void SetVar3f(const std::string& name, float v0, float v1, float v2) override;
	void SetVar2f(const std::string& name, float v0, float v1) override;
	void SetVar1f(const std::string& name, float v0) override;
	void SetVar4i(const std::string& name, int v0, int v1, int v2, int v3) override;
	void SetVar3i(const std::string& name, int v0, int v1, int v2) override;
	void SetVar2i(const std::string& name, int v0, int v1) override;
	void SetVar1i(const std::string& name, int v0) override;
	void SetVarMat4f(const std::string& name, const glm::mat4& mat) override;
	virtual void SetVar1b(const std::string& name, const bool v0) override;

#pragma endregion



	inline void Bind() const override  { glUseProgram(RendererID); };

	inline void UnBind() const override  { glUseProgram(0); };

	inline UINT32 getRenderID() { return RendererID; };

	inline std::string geFilename() { return Filename; };

private:
#pragma region Shader_Setup

	UINT32 CompileShader(const std::string& source, unsigned int type) override;

	Material::ShaderProgranSource PlaseShader(const std::string& filepath) override;

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



	std::map<std::string, int> UniformLocations;
};