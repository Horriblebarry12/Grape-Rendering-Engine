#include "Renderers/OpenGL/GLMaterial.h"
#include "Renderers/OpenGL/OpenGLRenderer.h"

#include "Mesh.h"
#include "Model.h"
#include <filesystem>
#include <chrono>
#include <Windows.h>
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_opengl3.h"
#include "ImGUI/imgui_impl_glfw.h"
#include "glm/gtc/matrix_transform.hpp"

#include "GLFW/glfw3.h"

glm::vec<2, double> PreCursorPos = glm::vec<2, double>(0.0);
glm::mat4 MoveCamera(glm::mat4 view, GLFWwindow* window)
{
	glm::vec<2, double> cursorPos = glm::vec<2, double>(0.0);

	glfwGetCursorPos(window, &cursorPos.x, &cursorPos.y);
	glm::vec<2, double> deltaCursorPos = cursorPos - PreCursorPos;

	view = glm::rotate_slow(view, (float)deltaCursorPos.x/1000.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	view = glm::rotate_slow(view, (float)deltaCursorPos.x/1000.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	

	int wKey = glfwGetKey(window, GLFW_KEY_W);
	int aKey = glfwGetKey(window, GLFW_KEY_A);
	int sKey = glfwGetKey(window, GLFW_KEY_S);
	int dKey = glfwGetKey(window, GLFW_KEY_D);

	view = glm::translate(view, glm::vec3((wKey -sKey)/2, 0.0f, (aKey -dKey) / 2));

	PreCursorPos = cursorPos;

	return view;
}

std::string GetCurrentDirectory()
{
	char buffer[MAX_PATH];
	GetModuleFileNameA(NULL, buffer, MAX_PATH);
	std::string::size_type pos = std::string(buffer).find_last_of("\\/");

	return std::string(buffer).substr(0, pos);
}

int main(void)
{

	spdlog::set_level(spdlog::level::debug);

	OpenGLRenderer renderer;
	renderer.Init();



	Mesh mesh = Mesh(std::vector<Vertex>(), std::vector<UINT32>(), std::vector<Texture>());

	mesh.vertices.push_back(Vertex(glm::vec3(0.5f, 0.5f, -0.1f), glm::vec3(0.0f), glm::vec2(1.0f)));
	mesh.vertices.push_back(Vertex(glm::vec3(0.5f, -0.5f, -0.1f), glm::vec3(0.0f), glm::vec2(1.0f, -1.0f)));
	mesh.vertices.push_back(Vertex(glm::vec3(-0.5f, -0.5f, -0.1f), glm::vec3(0.0f), glm::vec2(-1.0f)));
	mesh.vertices.push_back(Vertex(glm::vec3(-0.5f, 0.5f, -0.1f), glm::vec3(0.0f), glm::vec2(-1.0f, 1.0f)));

	mesh.indices.push_back(0);
	mesh.indices.push_back(1);
	mesh.indices.push_back(3);
	mesh.indices.push_back(1);
	mesh.indices.push_back(2);
	mesh.indices.push_back(3);

	GLMaterial mat("res\\shaders\\DefaultShader.shader");
	mesh.mat = &mat;
	renderer.SetupMesh(mesh);

	Model model = Model(GetCurrentDirectory() + "\\Test.blend");
	renderer.SetupModel(model);

	glm::mat4 Model = glm::mat4(1.0f);
	glm::mat4 View = glm::mat4(1.0f);
	glm::mat4 Projection = glm::mat4(1.0f);

	Projection = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);
	View = glm::translate(View, glm::vec3(0.0f, 0.0f, -3.0f));

	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(renderer.window, true);
	ImGui_ImplOpenGL3_Init("#version 330 core");

	UINT64 frame = 0.0f;
	float fov = 45.0f;
	float nearClip = 0.01f;
	float farClip = 100.0f;

	Texture tex = Texture();
	tex.path = "res\\Textures\\Thanos Kirby.png";
	renderer.SetupTexture(tex);
	model.meshes[0].mat = &mat;
	model.meshes[0].textures.push_back(tex);
    mat.Bind();
	mat.SetVar1i("TextureID", 0);

	glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	mat.Bind();
	mat.SetVarMat4f("u_Projection", Projection);


	while (!renderer.ShouldClose())
	{

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//View = MoveCamera(View, renderer.window);

		renderer.Clear();

		{


			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

			/*
			float ModelTranslate[3] {0.0f, 0.0f, 0.0f};
			ImGui::SliderFloat3("Model Translate", ModelTranslate, -0.1f, 0.1f);
			Model = glm::translate(Model, glm::vec3(ModelTranslate[0], ModelTranslate[1], ModelTranslate[2]));
			float ModelRotate[3] { 0.0f, 0.0f, 0.0f };
			ImGui::SliderFloat3("Model Rotate", ModelRotate, -0.1f, 0.1f);
			Model = glm::rotate(Model, ModelRotate[0], glm::vec3(1.0f, 0.0f, 0.0f));
			Model = glm::rotate(Model, ModelRotate[1], glm::vec3(0.0f, 1.0f, 0.0f));
			Model = glm::rotate(Model, ModelRotate[2], glm::vec3(0.0f, 0.0f, 1.0f));
			*/
			float ViewTranslate[3]{ 0.0f, 0.0f, 0.0f };
			ImGui::SliderFloat3("View Translate", ViewTranslate, -0.1f, 0.1f);
			View = glm::translate(View, glm::vec3(ViewTranslate[0], ViewTranslate[1], ViewTranslate[2]));
			float ViewRotate[3]{ 0.0f, 0.0f, 0.0f };
			ImGui::SliderFloat3("View Rotate", ViewRotate, -0.1f, 0.1f);
			View = glm::rotate(View, ViewRotate[0], glm::vec3(1.0f, 0.0f, 0.0f));
			View = glm::rotate(View, ViewRotate[1], glm::vec3(0.0f, 1.0f, 0.0f));
			View = glm::rotate(View, ViewRotate[2], glm::vec3(0.0f, 0.0f, 1.0f));
			mat.Bind();
			//mat.SetVarMat4f("u_Model", Model);
			mat.SetVarMat4f("u_View", View);
			
			


		}
		// renderer.DrawIndexed(vb);
		// renderer.DrawMesh(mesh);
		//renderer.DrawModel(model);
		
		for (int i = 0; i < 10; i++)
		{
			Model = glm::mat4(1.0f);
			Model = glm::translate(Model, cubePositions[i]);
			float angle = 20.0f * i;
			Model = glm::rotate(Model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			Model = glm::scale(Model, glm::vec3(0.5f));
			mat.Bind();
			mat.SetVarMat4f("u_Model", Model);
			renderer.DrawModel(model);
		}
		

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


		renderer.Finish();
		renderer.Poll();

		frame++;
	}
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	return 0;
}
