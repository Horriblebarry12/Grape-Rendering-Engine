#pragma once
#include "GrapePCH.h"

class Material
{
public:
	virtual void Bind() const = 0;
	virtual void UnBind() const = 0;

#pragma region Unifroms
	virtual void SetVar4f(const std::string& name, float v0, float v1, float v2, float v3) = 0;
	virtual void SetVar4f(const std::string& name, glm::vec4 vec4) = 0;
	virtual void SetVar3f(const std::string& name, float v0, float v1, float v2) = 0;
	virtual void SetVar2f(const std::string& name, float v0, float v1) = 0;
	virtual void SetVar1f(const std::string& name, float v0) = 0;
	virtual void SetVar4i(const std::string& name, int v0, int v1, int v2, int v3) = 0;
	virtual void SetVar3i(const std::string& name, int v0, int v1, int v2) = 0;
	virtual void SetVar2i(const std::string& name, int v0, int v1) = 0;
	virtual void SetVar1i(const std::string& name, int v0) = 0;
	virtual void SetVarMat4f(const std::string& name, const glm::mat4& mat) = 0;
	virtual void SetVar1b(const std::string& name, const bool v0) = 0;
#pragma endregion


protected:

	struct ShaderProgranSource
	{
		ShaderProgranSource(std::string vertexSource, std::string fragmentSource) : VertexSource(vertexSource), FragmentSource(fragmentSource)
		{

		}

		std::string VertexSource;
		std::string FragmentSource;
	};

	std::string Filename;

private:

	virtual UINT32 CompileShader(const std::string& source, unsigned int type) { return NULL; };

	virtual Material::ShaderProgranSource PlaseShader(const std::string& filepath) { return Material::ShaderProgranSource("", ""); };
};

