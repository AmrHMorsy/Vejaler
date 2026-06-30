#pragma once
#include "LightConfig.h"
#include "../../Shadow/Data/ShadowInfo.h"



struct LightInfo{
    uint32_t index;
    ShadowInfo shadow;
    LightConfig config;
    bool update = false;
    glm::uvec4 tileBounds;
    glm::vec4 boundingSphere;
    glm::mat4 projectionMatrix;
    std::array<glm::mat4, 6> cubemapViewMatrices;
};
