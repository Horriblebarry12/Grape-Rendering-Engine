#pragma once
#include <vector>
#include "GLDebug.h"
#include <GL\glew.h>

struct VertexBufferElement
{
public:
	unsigned int Type;
	unsigned int Count;
	unsigned char Normalized;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type) {
		case GL_FLOAT: return 4;
		case GL_UNSIGNED_INT: return 4;
		case GL_UNSIGNED_BYTE: return 1;
		}
		ASSERT(false);
		return 0;
	}
};

class VertexBufferLayout
{
public:

	VertexBufferLayout() : p_Stride(0) {};

	template<typename T>
	void Push(unsigned int count)
	{
		//static_assert(false);
	}

	template<>
	void Push<float>(unsigned int count)
	{
		p_Elements.push_back({ GL_FLOAT, count, GL_TRUE });
		p_Stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * count;
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		p_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		p_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
	}

	template<>
	void Push<unsigned char>(unsigned int count)
	{
		p_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		p_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count;
	}

	inline const std::vector<VertexBufferElement> GetElements() const& { return p_Elements; }
	inline unsigned int GetStride() const { return p_Stride;  }

private:
	std::vector<VertexBufferElement> p_Elements;
	unsigned int p_Stride;
};

