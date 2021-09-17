#include "VertexArray.h"
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "Debug.h"

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &p_RendererID))
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &p_RendererID))
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++) 
	{
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(0));
		GLCall(glVertexAttribPointer(i, element.Count, element.Type, element.Normalized, layout.GetStride(), (const void*)offset));
		offset += element.Count * VertexBufferElement::GetSizeOfType(element.Type);
	}
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(p_RendererID));
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}
