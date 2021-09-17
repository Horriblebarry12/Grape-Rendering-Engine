#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "BufferLayout.h"
#include "Debug.h"
#include "Shader.h"


//#include <stdlib.h>
//#include <stdio.h>
//#include <Windows.h>



int main(void)
{
	// TEMP



	//Logger::LogLine( TEXT("[Renderer Thread] Started Thread"));


	GLFWwindow* window;


	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(1280, 720, "ThisIsAWindow", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}


	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
	{
		return -1;
	}

	glfwSwapInterval(1);

	{
		float positions[] =
		{
			-0.5f, -0.5f, 0.0f, // 0
			0.5f, -0.5f, 0.0f, // 1
			0.5f, 0.5f, 0.0f, // 2
			-0.5f, 0.5f, 0.0f // 3
		};

		unsigned int indices[]
		{
			0, 1, 2,
			2, 3, 0
		};


		VertexArray va;
		VertexBuffer vb(positions, 4 * 3 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(3);
		va.AddBuffer(vb, layout);

		//glEnableVertexAttribArray(0);
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (const void*)0);

		IndexBuffer ib(indices, 6);

		Shader shader("res/shaders/DefaultShader.shader");
		shader.Bind();
		shader.SetUnform4f("u_ColorOffset", 0.2f, 0.3f, 0.1f, 1.0f);
		shader.Unbind();

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			glClear(GL_COLOR_BUFFER_BIT);

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

			if (glfwGetKey(window, GLFW_KEY_ESCAPE))
			{
				glfwSetWindowShouldClose(window, GLFW_TRUE);
			}
			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();

		}
	}

	glfwTerminate();
	return 0;
}