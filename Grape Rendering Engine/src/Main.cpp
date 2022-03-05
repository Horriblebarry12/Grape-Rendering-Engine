#include "Renderers/OpenGL/GLMaterial.h"
#include "Renderers/OpenGL/OpenGLRenderer.h"

#include "Mesh.h"
#include "Model.h"

#include <chrono>
#include "vendor/stb_image/stb_image.h"

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

	GLMaterial mat("res\\shaders\\TestShader.shader");
	mesh.mat = mat;
	renderer.SetupMesh(mesh);

	Model model = Model("C:\\Users\\harpi\\source\\repos\\Grape Rendering Engine\\x64\\Debug\\Test.blend");
	renderer.SetupModel(model);
	UINT64 frame = 0.0f;
	while (!renderer.ShouldClose())
	{
		renderer.Clear();
		mat.Bind();
		mat.SetVar1f("iTime", frame/100.0f);
		// renderer.DrawIndexed(vb);
		// renderer.DrawMesh(mesh);
		renderer.DrawModel(model);
		renderer.Finish();
		renderer.Poll();
		frame++;
	}


	return 0;
}