#pragma once
#include "../../../Includes.h"




struct DepthPrepassShaderUniform{
    
    struct Vertex{
        alignas(16) glm::mat4 cameraViewMatrix;
        alignas(16) glm::mat4 cameraProjectionMatrix;
    };
};
