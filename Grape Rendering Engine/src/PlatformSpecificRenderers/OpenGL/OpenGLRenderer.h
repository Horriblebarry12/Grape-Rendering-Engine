#pragma once
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "Material.h"

struct OpenGLVertexBuffer
{
public:

	OpenGLVertexBuffer(void* vertexData, unsigned int vertexSize, unsigned int* indexData, unsigned int indexCount)
	{
		// Vertex Buffer stuff (points)
		glGenBuffers(1, &VertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, vertexSize * sizeof(float), vertexData, GL_STATIC_DRAW);

		// Index Buffer stuff (triangles)
		IndexCount = indexCount;

		glGenBuffers(1, &IndexBufferID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indexData, GL_STATIC_DRAW);

		UseIndexes = true;
	}

	OpenGLVertexBuffer(void* vertexData, unsigned int vertexSize)
	{
		// Vertex Buffer stuff (points)
		glGenBuffers(1, &VertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, vertexSize * sizeof(float), vertexData, GL_STATIC_DRAW);
	}

	~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &VertexBufferID);

		glDeleteBuffers(1, &IndexBufferID);
	}

	void BindRendering() const
	{
		//Mat.Bind();
		
		glBindVertexArray(VertexArrayID);

		if (UseIndexes)
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferID);

	}

	inline bool GetUseIndexes() { return UseIndexes; };

	inline void SetUseIndexes(bool value) { UseIndexes = value; };

	inline void SetIndexData(unsigned int* indexData, unsigned int indexCount)
	{
		IndexData = indexData; 
		IndexCount = indexCount; 
	};

	inline UINT32 GetIndexCount() { return IndexCount; };

	void SetMaterial(const Material& mat)
	{
		//Mat = Material(mat);
	};

	void SetUpLayout()
	{
		glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);
		glGenVertexArrays(1, &VertexArrayID);
	};

	void BindVertexArray()
	{
		glBindVertexArray(VertexArrayID);
	};

	void BindBuffer()
	{
		glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);
	};

	void UpdateValues()
	{

	};

private:

	void* VertexData;

	unsigned int VertexSize;

	unsigned int* IndexData;

	unsigned int IndexCount;

	bool UseIndexes;

	Material Mat;

	UINT32 VertexBufferID = 0;

	UINT32 IndexBufferID = 0;

	UINT32 VertexArrayID = 0;
};

class OpenGLRenderer
{
public:

	OpenGLRenderer();

	enum DrawType
	{
		Triangle = 0x0004, // GL_TRIANGLE
		Lines = 0x0001 
	};

	void Init();

	void Clear();

	inline int ShouldClose()  { return glfwWindowShouldClose(window); };

	void DrawIndexed(OpenGLVertexBuffer& vb, DrawType drawType = DrawType::Triangle) const;

	inline void Finish() { glfwSwapBuffers(window); }

	inline void Poll() { glfwPollEvents(); }

	GLFWwindow* window;

	struct VertexArrayElement
	{
	public:
		unsigned int Type;
		unsigned int Count;
		unsigned char Normalized;

		static unsigned int GetSizeOfType(unsigned int type)
		{
			switch (type)
			{
			case GL_FLOAT: return 4;
			case GL_UNSIGNED_INT: return 4;
			case GL_UNSIGNED_BYTE: return 1;
			}
			return 0;
		};
	};

	class BufferLayout
	{
	public:
		template<typename T>
		void Push(unsigned int count, OpenGLVertexBuffer& vb)
		{
			
		}

		template<>
		void Push<float>(unsigned int count, OpenGLVertexBuffer& vb)
		{
			vb.SetUpLayout();
			vb.BindVertexArray();
			vb.BindBuffer();
			glEnableVertexAttribArray(NumOfElements);
			glVertexAttribPointer(NumOfElements, count, GL_FLOAT, GL_TRUE, VertexArrayElement::GetSizeOfType(GL_FLOAT) * count, (const void*)offset);
			offset += count * VertexArrayElement::GetSizeOfType(GL_FLOAT);
			NumOfElements++;
		}

		template<>
		void Push<unsigned int>(unsigned int count, OpenGLVertexBuffer& vb)
		{
			vb.SetUpLayout();
			vb.BindVertexArray();
			vb.BindBuffer();
			glEnableVertexAttribArray(NumOfElements);
			glVertexAttribPointer(NumOfElements, count, GL_UNSIGNED_INT, GL_TRUE, VertexArrayElement::GetSizeOfType(GL_UNSIGNED_INT) * count, (const void*)offset);
			offset += count * VertexArrayElement::GetSizeOfType(GL_UNSIGNED_INT);
			NumOfElements++;
		}

		template<>
		void Push<unsigned char>(unsigned int count, OpenGLVertexBuffer& vb)
		{
			vb.SetUpLayout();
			vb.BindVertexArray();
			vb.BindBuffer();
			glEnableVertexAttribArray(NumOfElements);
			glVertexAttribPointer(NumOfElements, count, GL_UNSIGNED_BYTE, GL_TRUE, VertexArrayElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count, (const void*)offset);
			offset += count * VertexArrayElement::GetSizeOfType(GL_UNSIGNED_BYTE);
			NumOfElements++;
		}

		UINT32 offset = 0;

		UINT32 NumOfElements = 0;

		std::vector<VertexArrayElement> elements;
	};
	
	BufferLayout& GetBufferLayout() { return Layout; };


	static void APIENTRY GLDebugMessageCallback(GLenum source, GLenum type, GLuint id,
		GLenum severity, GLsizei length,
		const GLchar* msg, const void* data);
private:

	BufferLayout Layout;
};