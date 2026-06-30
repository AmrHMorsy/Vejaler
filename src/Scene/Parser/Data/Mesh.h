#pragma once
#include "../../../Includes.h"
#include "../../../Core/Buffer/Data/UniformBufferPack.h"



struct Mesh {
    std::vector<uint32_t> indices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> textureCoordinates;
};
