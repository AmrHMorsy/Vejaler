#pragma once
#include "../../../Includes.h"



struct HDRToCubemapVertexShaderUniformVariables {
    alignas(16) glm::mat4 view ;
    alignas(16) glm::mat4 projection ;
};
