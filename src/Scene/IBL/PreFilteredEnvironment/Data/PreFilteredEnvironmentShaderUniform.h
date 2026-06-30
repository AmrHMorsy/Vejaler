#pragma once
#include "../../../../Includes.h"




struct PreFilteredEnvironmentShaderUniform{
    
    struct Vertex{
        alignas(16) glm::mat4 view;
        alignas(16) glm::mat4 projection;
    };
    
    struct Fragment{
        alignas(16) glm::vec4 roughnessAndresolution;
    };
    
};
