#pragma once
#include "../../../Includes.h"




struct OcclusionCullingShaderUniform{
    glm::vec2 screenResolution;
    glm::mat4 cameraViewMatrix;
    glm::mat4 cameraProjectionMatrix;
};
