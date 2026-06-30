#pragma once
#include "ShaderConfig.h"
#include "../../../Includes.h"



struct ShaderInfo{
    ShaderConfig config;
    std::vector<VkShaderModule> shaderModules ;
    std::vector<VkPipelineShaderStageCreateInfo> pipelineShaderStages;
};
