#include <string>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "BufferLayout.h"
#include "GLDebug.h"
#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_opengl3.h"
#include "ImGUI/imgui_impl_glfw.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

int iujsdhfgjhsdghjkaghmain(void)
{
	// TEMP

#if _DEBUG
	spdlog::set_level(spdlog::level::debug);
#else
	spdlog::set_level(spdlog::level::info);
#endif

	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
	{
		LogCritical("Glfw failed to initalize!");
		return -1;
	}

	window = glfwCreateWindow(1920, 1080, "ThisIsAWindow", NULL, NULL);
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
			// Front
			0.5f,  0.5f,
			0.5f, -0.5f,
			-0.5f, -0.5f,
			-0.5f,  0.5f

		};

		unsigned int indices[]
		{ // clockwise

			// Front
			0, 1, 3,   // first triangle
			1, 2, 3,    // second triangle

		};

		GLCall(glEnable(GL_BLEND));
		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		VertexArray va;
		VertexBuffer vb(verticies, 12 * sizeof(float));

		VertexBufferLayout layout;

		layout.Push<float>(2);
		//layout.Push<float>(1);
		
		va.AddBuffer(vb, layout);

		IndexBuffer ib(indices, 6);

		//glm::mat4 proj = glm::perspective(90.0f, 1920.0f / 1080.0f, 0.1f, 1000.0f);
		glm::mat4 proj = glm::ortho(-1.5f, 1.5f, -1.0f, 1.0f, -1.0f, 1.0f);



		Shader shader;

		//Texture texture("res\\Textures\\Thanos Kirby.png");
		shader.Bind();

		int texSlot = 0;
		//texture.Bind(texSlot);
		//shader.SetUniform1i("u_Texture", texSlot);

		va.Unbind();
		vb.Unbind();
		ib.Unbind();
		shader.Unbind();

		vb.~VertexBuffer();

		Renderer renderer;

		ImGui::CreateContext();

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 330 core");

		glm::vec3 translation(0, 0, 0);


		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.Clear();
			/*
			proj = glm::translate(proj, glm::vec3(0, 0, 0)); 
			proj = glm::rotate(proj, 0.0f, glm::vec3(1, 0, 0));
			proj = glm::rotate(proj, 0.01f, glm::vec3(0, 1, 0));
			proj = glm::rotate(proj, 0.0f, glm::vec3(0, 0, 1));
			*/
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			shader.Bind(); 
			glm::mat4 veiw = glm::translate(glm::mat4(1.0f), translation);

			{
				glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
				glm::mat4 mvp = proj * veiw * model;
				shader.SetUniformMat4f("u_MVP", mvp);
				renderer.Draw(va, ib, shader);
			}

			{
				glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.1, 0.1, 0));
				glm::mat4 mvp = proj * veiw * model;
				shader.SetUniformMat4f("u_MVP", mvp);
				renderer.Draw(va, ib, shader);
			}

			//model = glm::rotate(model, 0.01f, glm::vec3(0, 1, 0));


			shader.Unbind();

			{
				ImGui::Begin("Debug");
				ImGui::DragFloat3("Veiw Translation", &translation.x, 0.1f, -1920, 1920);
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
				ImGui::End();
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}