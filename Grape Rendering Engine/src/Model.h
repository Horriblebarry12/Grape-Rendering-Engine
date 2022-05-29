#pragma once
#include "GrapePCH.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

class Mesh;
struct Texture;

class GP_API Model
{
public:
    Model(std::string path)
    {
        loadModel(path);
    }
    // model data
    std::vector<Mesh> meshes;
    std::string directory;
private:
    std::vector<Texture> textures_loaded;

    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
        std::string typeName);
};

