#pragma once
#include "Data/Mesh.h"



class Parser{

private:
    
    static void TraverseMeshes(aiNode * node, const aiScene * assimpScene, aiNode * parent, std::vector<Mesh>& meshes, uint materialIndex);
    static Mesh ParseMesh(aiMesh * mesh, const aiScene * assimpScene);
    static std::vector<glm::vec3> ExtractVertices(aiMesh * mesh);
    static std::vector<glm::vec3> ExtractNormals(aiMesh * mesh);
    static std::vector<glm::vec2> ExtractTextureCoordinates(aiMesh * mesh);
    static std::vector<uint32_t> ExtractIndices(aiMesh * mesh);
    
public:
    
    static bool Parse(const aiScene * assimpScene, std::vector<Mesh>& meshes, uint materialIndex);
};
