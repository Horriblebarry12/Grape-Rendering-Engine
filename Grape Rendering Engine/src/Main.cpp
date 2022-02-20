#include "PlatformSpecificRenderers/OpenGL/Material.h"
#include "PlatformSpecificRenderers/OpenGL/OpenGLRenderer.h"

#include <chrono>
#include "vendor/stb_image/stb_image.h"
#include "Mesh.h"

int main(void)
{


	unsigned int indices[]
	{ // clockwise
		0, 1, 3,   // first triangle
		1, 2, 3,    // second triangle

	};

	//TODO: fix this

	Mesh mesh("C:\\Users\\harpi\\source\\repos\\Grape Rendering Engine\\x64\\Debug\\Test.blend");

	OpenGLRenderer renderer;
	renderer.Init();

	float* verticies = new float[mesh.scene->mMeshes[0]->mNumVertices*3];
	{
		int n = 0;
		for (size_t i = 0; i < mesh.scene->mMeshes[0]->mNumVertices * 3; i += 3)
		{
			verticies[i] = mesh.scene->mMeshes[n]->mVertices[n].x;
			verticies[i+1] = mesh.scene->mMeshes[n]->mVertices[n].y;
			verticies[i+2] = mesh.scene->mMeshes[n]->mVertices[n].z;
		}
		n++;
	}
	OpenGLVertexBuffer vb(verticies, mesh.scene->mMeshes[0]->mNumVertices, mesh.scene->mMeshes[0]->mFaces->mIndices, mesh.scene->mMeshes[0]->mFaces->mNumIndices);

	Material mat("res\\shaders\\TestShader.shader");
	mat.Bind();
	renderer.GetBufferLayout().Push<float>(3, vb);

	vb.SetMaterial(mat);
	
	std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch());

	float frame = 0.0f;
	while (!renderer.ShouldClose())
	{
		renderer.Clear();
		mat.Bind();
		mat.SetUniform1f("iTime", frame/100.0f);
		renderer.DrawIndexed(vb);
		renderer.Finish();
		renderer.Poll();
		std::cout << frame << std::endl;
		frame++;
		ms = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch());
	}


	return 0;
}