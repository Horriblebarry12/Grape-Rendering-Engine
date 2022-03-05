#include "Mesh.h"
#include "GL/glew.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<UINT32> indices, std::vector<Texture> textures)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
}