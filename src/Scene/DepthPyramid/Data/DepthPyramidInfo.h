#pragma once
#include "../../../Includes.h"
#include "DepthPyramidShaderUniform.h"




struct DepthPyramidInfo{
    ShaderInfo shader;
    VkSampler sampler;
    uint32_t numMipLevels;
    glm::uvec2 numWorkGroups;
    VkPipeline computePipeline;
    std::vector<VkFence> fences;
    std::vector<ImageInfo> images;
    VkDescriptorPool descriptorPool;
    VkPipelineLayout pipelineLayout;
    VkCommandPool primaryCommandPool;
    DepthPyramidShaderUniform uniformData;
    std::vector<VkImageView> fullImageViews;
    VkDescriptorSetLayout descriptorSetLayout;
    std::vector<VkSemaphore> finishedSemaphores;
    std::vector<VkDescriptorSet> descriptorSets;
    std::vector<UniformBufferPack> uniformBuffers;
    std::vector<VkCommandBuffer> primaryCommandBuffers;
    std::vector<std::vector<VkImageView>> mipImageViews;
    std::vector<std::vector<DescriptorBinding>> descriptorBindings;
    std::vector<std::vector<VkDescriptorImageInfo>> mipDescriptorImages;
};
