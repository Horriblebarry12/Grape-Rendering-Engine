#include "Texture.h"
#include "GLDebug.h"
#include "GL/glew.h"
#include "stb_image/stb_image.h"

glTexture::glTexture(const std::string& path) : p_Filepath(path), p_LocalBuffer(nullptr), p_Width(0), p_Height(0), p_BPP(0)
{

	stbi_set_flip_vertically_on_load(1);
	p_LocalBuffer = stbi_load(path.c_str(), &p_Width, &p_Height, &p_BPP, STBI_rgb_alpha);

	GLCall(glGenTextures(1, &p_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, p_RendererID));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, p_Width, p_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, p_LocalBuffer));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	if (p_LocalBuffer)
	{
		stbi_image_free(p_LocalBuffer);
	}
}

glTexture::~glTexture()
{
	GLCall(glDeleteTextures(1, &p_RendererID));
}

void glTexture::Bind(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, p_RendererID));
}

void glTexture::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
