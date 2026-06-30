#pragma once
#include "../../../Includes.h"
#include "OcclusionCullingShaderUniform.h"




struct OcclusionCullingInfo{
    ShaderInfo shader;
    VkPipeline computePipeline;
    std::vector<VkFence> fences;
    VkDescriptorPool descriptorPool;
    VkPipelineLayout pipelineLayout;
    VkCommandPool primaryCommandPool;
    OcclusionCullingShaderUniform uniformData;
    VkDescriptorSetLayout descriptorSetLayout;
    std::vector<VkSemaphore> finishedSemaphores;
    std::vector<VkDescriptorSet> descriptorSets;
    std::vector<UniformBufferPack> uniformBuffers;
    std::vector<BufferPack> outputDrawCommandsBuffers;
    std::vector<VkCommandBuffer> primaryCommandBuffers;
    std::vector<BufferPack> outputDrawCommandsCountBuffers;
    std::vector<BufferPack> outputDrawCommandsCountStageBuffers;
    std::vector<std::vector<DescriptorBinding>> descriptorBindings;
    std::vector<std::vector<VkDescriptorBufferInfo>> outputDrawCommandsCountDescriptorBuffers;
    std::vector<std::vector<VkDescriptorBufferInfo>> outputDrawCommandsDescriptorBuffers;
};
