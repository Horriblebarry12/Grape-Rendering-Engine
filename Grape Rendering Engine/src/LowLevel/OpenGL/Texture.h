#pragma once
#include <string>

class Texture
{
public:
	Texture(const std::string& path);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int GetWidth() const { return p_Width; }
	inline int GetHeight() const { return p_Height; }
private:
	unsigned int p_RendererID;
	std::string p_Filepath;
	unsigned char* p_LocalBuffer;
	int p_Width, p_Height, p_BPP;
};

