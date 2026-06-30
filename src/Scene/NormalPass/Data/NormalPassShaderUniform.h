#pragma once
#include "../../../Includes.h"



struct NormalPassShaderUniform{
    
    struct Vertex{
        alignas(16) glm::mat4 cameraViewMatrix;
        alignas(16) glm::mat4 cameraProjectionMatrix;
    };
    
    struct Fragment{
        float mipLevel;
    };
};
