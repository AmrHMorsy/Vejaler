#pragma once
#include "../../../Includes.h"



struct FrustumCullingUniformVariables{
    alignas(16) std::array<glm::vec4, 6> planes;
    alignas(16) glm::vec4 parameters;
};
