#pragma once
#include "../../../Includes.h"



struct SkyboxVertexShaderUniformVariables {
    alignas(16) glm::vec4 mipLevel_isHDRTonemappingEnabled_isGammaCorrectEnabled;
    alignas(16) glm::mat4 view ;
    alignas(16) glm::mat4 projection ;
} ;
