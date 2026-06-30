#pragma once
#include "../../Includes.h"



class ComputePipeline{
    
public:
    
    static std::vector<VkPipeline> Create(VkDevice logicalDevice, VkPipelineLayout pipelineLayout, VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfo, uint32_t numComputePipelines);
    static void Bind(VkCommandBuffer commandBuffer, VkPipeline computePipeline);
};
