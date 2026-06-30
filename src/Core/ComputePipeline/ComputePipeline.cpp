#include "ComputePipeline.h"





std::vector<VkPipeline> ComputePipeline::Create(VkDevice logicalDevice, VkPipelineLayout pipelineLayout, VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfo, uint32_t numComputePipelines)
{
    VkComputePipelineCreateInfo computePipelineCreateInfo{};
    
    computePipelineCreateInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    computePipelineCreateInfo.stage = pipelineShaderStageCreateInfo;
    computePipelineCreateInfo.layout = pipelineLayout;
    
    std::vector<VkPipeline> computePipelines(numComputePipelines);
    
    VkResult result = vkCreateComputePipelines(logicalDevice, VK_NULL_HANDLE, numComputePipelines, &computePipelineCreateInfo, nullptr, computePipelines.data());
    
    if(result != VK_SUCCESS)
        throw std::runtime_error("\033[31m\nERROR::FAILURE TO CREATE COMPUTE PIPELINES\033[0m");
    
    return computePipelines;
}

void ComputePipeline::Bind(VkCommandBuffer commandBuffer, VkPipeline computePipeline)
{
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, computePipeline);
}
