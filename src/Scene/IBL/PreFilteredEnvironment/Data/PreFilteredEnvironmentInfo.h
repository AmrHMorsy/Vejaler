#pragma once
#include "../../../../Includes.h"
#include "../../../../Core/Core.h"
#include "PreFilteredEnvironmentConfig.h"
#include "../../../Texture/Data/TexturePack.h"
#include "PreFilteredEnvironmentShaderUniform.h"
#include "../../../../Core/GraphicsPipeline/Data/GraphicsPipelineConfig.h"


#include "../../../../Core/Sampler/Data/SamplerConfig.h"

    
struct PreFilteredEnvironmentInfo{
    VkFence fence;
    VkSampler sampler;
    ShaderInfo shader;
    uint32_t resolution;
    ImageInfo image;
    VkImageView imageView;
    SamplerConfig samplerConfig;
    uint32_t numMipLevels;
    VkRenderPass renderPass;
    BufferPack vertexBuffer;
    VertexInputInfo vertexInput;
    VkPipelineLayout pipelineLayout;
    VkDescriptorPool descriptorPool;
    VkCommandPool primaryCommandPool;
    std::vector<std::thread> threads;
    std::vector<VkExtent2D> resolutions;
    PreFilteredEnvironmentConfig config;
    VkCommandBuffer primaryCommandBuffer;
    GraphicsPipelineInfo graphicsPipeline;
    std::vector<VkFramebuffer> frameBuffers;
    std::vector<VkImageView> bakingImageViews;
    VkDescriptorSetLayout descriptorSetLayout;
    std::vector<VkDescriptorSet> descriptorSets;
    GraphicsPipelineConfig graphicsPipelineConfig;
    std::vector<VkCommandPool> secondaryCommandPools;
    std::vector<VkCommandBuffer> secondaryCommandBuffers;
    std::vector<DescriptorBinding> descriptorBindingSchematic;
    std::vector<UniformBufferPack> vertexShaderUniformBuffers;
    std::vector<UniformBufferPack> fragmentShaderUniformBuffers;
    std::vector<std::vector<DescriptorBinding>> descriptorBindings;
    std::vector<PreFilteredEnvironmentShaderUniform::Fragment> fragmentShaderDatas;
    std::vector<PreFilteredEnvironmentShaderUniform::Vertex> vertexShaderDatas;
};
