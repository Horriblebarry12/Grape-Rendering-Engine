#include "Grape.h"
#include "GTime.h" 

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
#include "vendor/glm/gtc/matrix_transform.hpp"

#include "GLFW/glfw3.h"

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, -3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

glm::mat4 MoveCamera(glm::mat4 view, GLFWwindow* window)
{
	const float cameraSpeed = 2.5f * GTime::DeltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	return view;
}

bool firstMouse = true;
double lastX = 1080.0 / 2.0;
double lastY = 1920.0 / 2.0;

float pitch = 0.0f, yaw = 0.0f;
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT_ALT))
	{
		lastX = xpos;
		lastY = ypos;

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		return;
	}
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);
}

std::string GetCurrentDirectory()
{
	char buffer[MAX_PATH];
	GetModuleFileNameA(NULL, buffer, MAX_PATH);
	std::string::size_type pos = std::string(buffer).find_last_of("\\/");

	return std::string(buffer).substr(0, pos);
}

int _main_(void)
{

	spdlog::set_level(spdlog::level::debug);

	OpenGLRenderer renderer;
	renderer.Init();

	GTime::Init(glfwGetTime());

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


	glfwSetCursorPosCallback(renderer.window, mouse_callback);



	while (!renderer.ShouldClose())
	{

		GTime::Tick(glfwGetTime());
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		View = MoveCamera(View, renderer.window);

		renderer.Clear();

		View = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

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

			float ViewTranslate[3]{ 0.0f, 0.0f, 0.0f };
			ImGui::SliderFloat3("View Translate", ViewTranslate, -0.1f, 0.1f);
			View = glm::translate(View, glm::vec3(ViewTranslate[0], ViewTranslate[1], ViewTranslate[2]));
			float ViewRotate[3]{ 0.0f, 0.0f, 0.0f };
			ImGui::SliderFloat3("View Rotate", ViewRotate, -0.1f, 0.1f);
			View = glm::rotate(View, ViewRotate[0], glm::vec3(1.0f, 0.0f, 0.0f));
			View = glm::rotate(View, ViewRotate[1], glm::vec3(0.0f, 1.0f, 0.0f));
			View = glm::rotate(View, ViewRotate[2], glm::vec3(0.0f, 0.0f, 1.0f));
			*/
			//mat.SetVarMat4f("u_Model", Model);
			mat.Bind();
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
			Model = glm::rotate(Model, glm::radians((float)GTime::AppTime * 10.0f), glm::normalize(glm::vec3(std::rand(), std::rand(), std::rand())));
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
