#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <math.h>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "BufferLayout.h"
#include "Debug.h"
#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"

//#include <stdlib.h>
//#include <stdio.h>
//#include <Windows.h>



int main(void)
{
	// TEMP

#if _DEBUG
	spdlog::set_level(spdlog::level::debug);
#elif
	spdlog::set_level(spdlog::level::info);
#endif


	//Logger::LogLine( TEXT("[Renderer Thread] Started Thread"));


	GLFWwindow* window;


	/* Initialize the library */
	if (!glfwInit())
	{
		LogCritical("Glfw failed to initalize!");
		return -1;
	}

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(1280, 720, "ThisIsAWindow", NULL, NULL);
	if (!window)
	{
		LogCritical("Window failed to create!");
		glfwTerminate();
		return -1;
	}
	

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	
	if (glewInit() != GLEW_OK)
	{
		LogCritical("Glew failed to initalize!");
		return -1;
	}

	glfwSwapInterval(1);

	{
		float verticies[] =
		{
			-0.5f, -0.5f,  0.0f, 0.0f,
			0.5f,  -0.5f,  1.0f, 0.0f,
			0.5f,   0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.0f, 1.0f
		};

		unsigned int indices[]
		{
			0, 1, 2,
			2, 3, 0
		};

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		VertexArray va;
		VertexBuffer vb(verticies, 16 * sizeof(float));

		VertexBufferLayout layout;

		layout.Push<float>(2);		
		layout.Push<float>(2);
		
		va.AddBuffer(vb, layout);

		IndexBuffer ib(indices, 6);

		Shader shader;

		Texture texture("C:\\Users\\harpi\\Pictures\\Epic Stuff\\barry.png");
		shader.Bind();
		texture.Bind(0);
		shader.SetUnform1i("u_Texture", 0);

		va.Unbind();
		vb.Unbind();
		ib.Unbind();
		shader.Unbind();

		Renderer renderer;

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.Clear();

			renderer.Draw(va, ib, shader);

			//shader.SetUnform4f("u_Color", (sin(glfwGetTime()) + 1) / 2, (cos(glfwGetTime()) + 1) / 2, (sin(glfwGetTime() + 60) + 1)/2, 1.0f);
			


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