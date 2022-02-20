#include "Mesh.h"

#include "assimp/postprocess.h"

Mesh::Mesh(std::string MeshLocation)
{

    scene = importer.ReadFile(MeshLocation, 
        aiProcess_CalcTangentSpace |
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_SortByPType );

    if (scene == nullptr)
    {
        // TODO: Log this error.
    }



}
