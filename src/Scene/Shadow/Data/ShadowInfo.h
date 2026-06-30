#pragma once
#include "ShadowConfig.h"
#include "../../../Includes.h"
#include "../../../Core/Core.h"





struct ShadowInfo{
    VkFence fence;
    ShaderInfo shader;
    ShadowConfig config;
    ImageInfo image;
    VkSemaphore semaphore;
    VkImageView imageView;
    VkRenderPass renderPass;
    VertexInputInfo vertexInput;
    VkPipelineLayout pipelineLayout;
    VkCommandPool primaryCommandPool;
    std::vector<VkExtent2D> resolutions;
    VkCommandBuffer primaryCommandBuffer;
    GraphicsPipelineInfo graphicsPipeline;
    std::vector<VkFramebuffer> frameBuffers;
    std::vector<VkImageView> bakingImageViews;
    VkDescriptorSetLayout descriptorSetLayout;
    std::vector<VkCommandBuffer> commandBuffers;
    GraphicsPipelineConfig graphicsPipelineConfig;
    std::vector<VkDescriptorPool> descriptorPools;
    std::vector<std::vector<VkDescriptorSet>> descriptorSets;
    std::vector<DescriptorBinding> descriptorBindingSchematic;
    std::vector<std::vector<UniformBufferPack>> vertexShaderUniformBuffers;
    std::vector<std::vector<UniformBufferPack>> fragmentShaderUniformBuffers;
    std::vector<std::vector<std::vector<DescriptorBinding>>> descriptorBindings;
    std::vector<std::vector<HostVisibleBuffer>> modelMatricesBuffers;
};
