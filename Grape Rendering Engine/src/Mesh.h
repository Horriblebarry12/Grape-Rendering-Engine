#pragma once
#include "GrapePCH.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"

class Mesh
{
public:
	Mesh(std::string MeshLocation);

	const aiScene* scene;

	Assimp::Importer importer;
};

