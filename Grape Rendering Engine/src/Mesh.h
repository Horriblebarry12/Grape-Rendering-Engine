#pragma once
#include "GrapePCH.h"
#include "Renderers/Material.h"
struct GP_API Vertex
{
    Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texCoords) : Position(position), Normal(normal), TexCoords(texCoords)
    {

    }

    Vertex() {};

    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct GP_API Texture
{
    unsigned int id;
    std::string type;
    std::string path;
    bool isSetup;
};



class GP_API Mesh
{
public:
    // mesh data
    std::vector<Vertex>       vertices;
    std::vector<UINT32> indices;
    std::vector<Texture>      textures;
    
    Material* mat;

    Mesh(std::vector<Vertex> vertices, std::vector<UINT32> indices, std::vector<Texture> textures);
    /// <summary>
    /// For OpenGL renderer, do not edit
    /// </summary>
    unsigned int VAO, VBO, EBO;
};

