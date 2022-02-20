#include "Material.h"
void Material::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

void Material::SetUniform1f(const std::string& name, float v0)
{
	glUniform1f(GetUniformLocation(name), v0);
}

void Material::SetUniform1i(const std::string& name, int v0)
{
	glUniform1i(GetUniformLocation(name), v0);
}

void Material::SetUniformMat4f(const std::string& name, const glm::mat4& mat)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
}

int Material::GetUniformLocation(const std::string& name)
{
	if (UniformLocations.count(name) > 0)
	{
		return UniformLocations[name];
	}

	int location = glGetUniformLocation(RendererID, name.c_str());
	if (location == -1)
	{
		// LogError("Couldn't find uniform \"{}\"", name);
		__debugbreak();

	}
	UniformLocations.insert(std::pair<std::string, int>(name, location));

	return location;
}