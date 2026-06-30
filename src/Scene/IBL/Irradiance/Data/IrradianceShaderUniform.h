#pragma once
#include "../../../../Includes.h"




struct IrradianceShaderUniform{
    struct Vertex{
        alignas(16) glm::mat4 view;
        alignas(16) glm::mat4 projection;
    };
};
