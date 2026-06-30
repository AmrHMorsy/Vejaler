#pragma once
#include "ModelConfig.h"
#include "../../Material/Data/MaterialInfo.h"




struct ModelInfo{
    int index;
    ModelConfig config;
    int boundingBoxIndex;
    MaterialInfo material;
    int selectedAlbedo = 0;
    TransformInfo transform;
    glm::vec4 boundingSphere;
    std::vector<Mesh> meshes;
    uint32_t firstInstance = 0;
    BoundingBoxInfo boundingBox;
    std::vector<uint32_t> indices;
    std::vector<glm::vec3> vertices;
};
