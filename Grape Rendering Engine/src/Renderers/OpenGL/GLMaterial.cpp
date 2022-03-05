#include "GLMaterial.h"
void GLMaterial::SetVar4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

void GLMaterial::SetVar4f(const std::string& name, glm::vec4 vec4)
{
	glUniform4f(GetUniformLocation(name), vec4.x, vec4.y, vec4.z, vec4.w);
}

void GLMaterial::SetVar3f(const std::string& name, float v0, float v1, float v2)
{
	glUniform3f(GetUniformLocation(name), v0, v1, v2);
}

void GLMaterial::SetVar2f(const std::string& name, float v0, float v1)
{
	glUniform2f(GetUniformLocation(name), v0, v1);

}

void GLMaterial::SetVar1f(const std::string& name, float v0)
{
	glUniform1f(GetUniformLocation(name), v0);
}

void GLMaterial::SetVar4i(const std::string& name, int v0, int v1, int v2, int v3)
{
	glUniform4i(GetUniformLocation(name), v0, v1, v2, v3);
}

void GLMaterial::SetVar3i(const std::string& name, int v0, int v1, int v2)
{
	glUniform3i(GetUniformLocation(name), v0, v1, v2);
}

void GLMaterial::SetVar2i(const std::string& name, int v0, int v1)
{
	glUniform2i(GetUniformLocation(name), v0, v1);

}

void GLMaterial::SetVar1i(const std::string& name, int v0)
{
	glUniform1i(GetUniformLocation(name), v0);
}

void GLMaterial::SetVarMat4f(const std::string& name, const glm::mat4& mat)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
}

UINT32 GLMaterial::CompileShader(const std::string& source, unsigned int type)
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

Material::ShaderProgranSource GLMaterial::PlaseShader(const std::string& filepath)
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

int GLMaterial::GetUniformLocation(const std::string& name)
{
	if (UniformLocations.count(name) > 0)
	{
		return UniformLocations[name];
	}

	int location = glGetUniformLocation(RendererID, name.c_str());
	if (location == -1)
	{
		LogError("Couldn't find uniform \"{}\"", name);
	}
	UniformLocations.insert(std::pair<std::string, int>(name, location));

	return location;
}