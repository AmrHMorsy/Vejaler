#include "Parser.h"





bool Parser::Parse(const aiScene * assimpScene, std::vector<Mesh>& meshes, uint materialIndex)
{
    TraverseMeshes(assimpScene->mRootNode, assimpScene, NULL, meshes, materialIndex);
    
    return true;
}

void Parser::TraverseMeshes(aiNode * node, const aiScene * assimpScene, aiNode * parent, std::vector<Mesh>& meshes, uint materialIndex)
{
    for(unsigned int i = 0; i < node->mNumMeshes; i++){
        aiMesh * mesh = assimpScene->mMeshes[node->mMeshes[i]];
        if(mesh->mMaterialIndex == materialIndex)
            meshes.push_back(ParseMesh(mesh, assimpScene));
    }
    
    for(unsigned int i = 0; i < node->mNumChildren; i++)
        TraverseMeshes(node->mChildren[i], assimpScene, node, meshes, materialIndex) ;
}

Mesh Parser::ParseMesh(aiMesh * mesh, const aiScene * scene)
{
    Mesh m;
    
    m.vertices = ExtractVertices(mesh);
    m.normals = ExtractNormals(mesh);
    m.textureCoordinates = ExtractTextureCoordinates(mesh);
    m.indices = ExtractIndices(mesh);
    
    return m;
}

std::vector<glm::vec3> Parser::ExtractVertices(aiMesh * mesh)
{
    std::vector<glm::vec3> vertices ;
    for(unsigned int j = 0; j < mesh->mNumVertices; j++)
        vertices.push_back(glm::vec3(mesh->mVertices[j].x, mesh->mVertices[j].y, mesh->mVertices[j].z));
    
    return vertices ;
}

std::vector<glm::vec3> Parser::ExtractNormals(aiMesh * mesh)
{
    std::vector<glm::vec3> normals ;
    for( unsigned int j = 0 ; j < mesh->mNumVertices ; j++ )
        normals.push_back(glm::normalize(glm::vec3(mesh->mNormals[j].x, mesh->mNormals[j].y, mesh->mNormals[j].z))) ;

    return normals ;
}

std::vector<glm::vec2> Parser::ExtractTextureCoordinates(aiMesh * mesh)
{
    std::vector<glm::vec2> textureCoordinates ;
    for( unsigned int j = 0 ; j < mesh->mNumVertices ; j++ )
        textureCoordinates.push_back(glm::vec2(mesh->mTextureCoords[0][j].x, 1.0f - mesh->mTextureCoords[0][j].y)) ;

    return textureCoordinates;
}

std::vector<uint32_t> Parser::ExtractIndices(aiMesh * mesh)
{
    std::vector<uint32_t> indices ;
    
    for( unsigned int j = 0 ; j < mesh->mNumFaces ; j++ ) {
        for( unsigned int k = 0 ; k < mesh->mFaces[j].mNumIndices ; k++ )
            indices.push_back( mesh->mFaces[j].mIndices[k] ) ;
    }
    
    return indices ;
}
