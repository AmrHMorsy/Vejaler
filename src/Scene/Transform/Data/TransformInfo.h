#pragma once
#include "../../../Includes.h"
#include "TransformConfig.h"




struct TransformInfo{
    float maxScale = 1.0f;
    TransformConfig config;
    glm::mat4 modelMatrix = glm::mat4(1.0f);    
};
