#include "Renderers/OpenGL/GLMaterial.h"
#include "Renderers/OpenGL/OpenGLRenderer.h"

#include "Mesh.h"
#include "Model.h"
#include <filesystem>
#include <chrono>
#include "vendor/stb_image/stb_image.h"
#include <Windows.h>
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_opengl3.h"
#include "ImGUI/imgui_impl_glfw.h"
#include "glm/gtc/matrix_transform.hpp"

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

	mesh.vertices.push_back(Vertex(glm::vec3(0.5f, 0.5f, -0.1f), glm::vec3(0.0f), glm::vec2(0.0f)));
	mesh.vertices.push_back(Vertex(glm::vec3(0.5f, -0.5f, -0.1f), glm::vec3(0.0f), glm::vec2(0.0f)));
	mesh.vertices.push_back(Vertex(glm::vec3(-0.5f, -0.5f, -0.1f), glm::vec3(0.0f), glm::vec2(0.0f)));
	mesh.vertices.push_back(Vertex(glm::vec3(-0.5f, 0.5f, -0.1f), glm::vec3(0.0f), glm::vec2(0.0f)));

	mesh.indices.push_back(0);
	mesh.indices.push_back(1);
	mesh.indices.push_back(3);
	mesh.indices.push_back(1);
	mesh.indices.push_back(2);
	mesh.indices.push_back(3);

	GLMaterial mat("res\\shaders\\DefaultShader.shader");
	mesh.mat = mat;
	renderer.SetupMesh(mesh);

	Model model = Model(GetCurrentDirectory() + "\\Test.blend");
	renderer.SetupModel(model);

	glm::mat4 Model = glm::mat4(1.0f);
	glm::mat4 View = glm::mat4(1.0f);
	glm::mat4 Projection = glm::perspective(45.0f, 1920.0f / 1080.0f, 1.0f, 90.0f);

	ImGui::CreateContext();

	ImGui_ImplGlfw_InitForOpenGL(renderer.window, true);
	ImGui_ImplOpenGL3_Init("#version 330 core");

	UINT64 frame = 0.0f;
	while (!renderer.ShouldClose())
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		renderer.Clear();
		mat.Bind();
		mat.SetVarMat4f("u_MVP", Model * View * Projection);
		// renderer.DrawIndexed(vb);
		// renderer.DrawMesh(mesh);

		{
			ImGui::Begin("Debug");
			float ViewT[3] = {0.0f, 0.0f, 0.0f};
			ImGui::DragFloat3("Translate View", ViewT, 0.01f, -10, 10);
			View = glm::translate(View, glm::vec3(ViewT[0], ViewT[1], ViewT[2]));

			float ViewR[3] = { 0.0f, 0.0f, 0.0f };
			ImGui::DragFloat3("Rotate View", ViewR, 0.01f, -10, 10);
			View = glm::rotate(View, ViewR[0], glm::vec3(1.0f, 0.0f, 0.0f));
			View = glm::rotate(View, ViewR[1], glm::vec3(0.0f, 1.0f, 0.0f));
			View = glm::rotate(View, ViewR[2], glm::vec3(0.0f, 0.0f, 1.0f));

			float ModelT[3] = { 0.0f, 0.0f, 0.0f };
			ImGui::DragFloat3("Translate Model", ModelT, 0.01f, -10, 10);
			Model = glm::translate(Model, glm::vec3(ModelT[0], ModelT[1], ModelT[2]));

			ImGui::End();
		}
		renderer.DrawModel(model);
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