#pragma once
#include "GrapePCH.h"

class Material
{
public:
	virtual void Bind() const { };
	virtual void UnBind() const { };

#pragma region MyRegion
	virtual void SetVar4f(const std::string& name, float v0, float v1, float v2, float v3) { };
	virtual void SetVar4f(const std::string& name, glm::vec4 vec4) { };
	virtual void SetVar3f(const std::string& name, float v0, float v1, float v2) { };
	virtual void SetVar2f(const std::string& name, float v0, float v1) { };
	virtual void SetVar1f(const std::string& name, float v0) { };
	virtual void SetVar4i(const std::string& name, int v0, int v1, int v2, int v3) { };
	virtual void SetVar3i(const std::string& name, int v0, int v1, int v2) { };
	virtual void SetVar2i(const std::string& name, int v0, int v1) { };
	virtual void SetVar1i(const std::string& name, int v0) { };
	virtual void SetVarMat4f(const std::string& name, const glm::mat4& mat) { };
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

