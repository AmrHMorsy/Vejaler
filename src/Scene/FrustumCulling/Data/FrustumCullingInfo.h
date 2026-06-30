#pragma once
#include "../../../Includes.h"
#include "../../../Core/Shader/Data/ShaderInfo.h"
#include "../../../Core/Buffer/Data/UniformBufferPack.h"
#include "../../../Core/Buffer/Data/HostVisibleBuffer.h"
#include "../../../Core/DescriptorBinding/Data/DescriptorBinding.h"
#include "FrustumCullingUniformVariables.h"


struct FrustumCullingInfo{
    ShaderInfo shader;
    VkPipeline computePipeline;
    std::vector<VkFence> fences;
    VkDescriptorPool descriptorPool;
    VkPipelineLayout pipelineLayout;
    VkCommandPool primaryCommandPool;
    std::vector<std::thread> threads;
    FrustumCullingUniformVariables uniformData;
    VkDescriptorSetLayout descriptorSetLayout;
    std::vector<VkSemaphore> finishedSemaphores;
    std::vector<VkDescriptorSet> descriptorSets;
    std::vector<UniformBufferPack> uniformBuffers;
    std::vector<BufferPack> allDrawCommandsBuffers;
    std::vector<VkCommandBuffer> primaryCommandBuffers;
    std::vector<BufferPack> visibleDrawCommandsBuffers;
    std::vector<BufferPack> visibleDrawCommandsCountBuffers;
    std::vector<BufferPack> visibleDrawCommandsCountStageBuffers;
    std::vector<std::vector<DescriptorBinding>> descriptorBindings;
    std::vector<std::vector<VkDescriptorBufferInfo>> visibleDrawCommandsDescriptorBuffers;
};
