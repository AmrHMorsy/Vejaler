#pragma once
#include "../../../Includes.h"



struct ShadowShaderUniform{
    struct Vertex{
        alignas(16) glm::mat4 viewMatrix;
        alignas(16) glm::mat4 projectionMatrix;
    };
    struct Fragment{
        alignas(16) glm::vec4 lightPosition;
        alignas(16) glm::vec4 lightFarPlane;
    };
};
