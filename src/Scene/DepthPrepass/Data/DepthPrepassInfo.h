#pragma once
#include "../../../Includes.h"
#include "DepthPrepassShaderUniform.h"
#include "../../../Core/Image/Data/ImageInfo.h"
#include "../../../Core/Shader/Data/ShaderInfo.h"
#include "../../../Core/Sampler/Data/SamplerConfig.h"
#include "../../../Core/Buffer/Data/UniformBufferPack.h"
#include "../../../Core/Buffer/Data/HostVisibleBuffer.h"
#include "../../../Core/VertexInput/Data/VertexInputInfo.h"
#include "../../../Core/DescriptorBinding/Data/DescriptorBinding.h"
#include "../../../Core/GraphicsPipeline/Data/GraphicsPipelineInfo.h"
#include "../../../Core/GraphicsPipeline/Data/GraphicsPipelineConfig.h"




struct DepthPrepassInfo{
    VkSampler sampler;
    ShaderInfo shader;
    VkRenderPass renderPass;
    VertexInputInfo vertexInput;
    std::vector<VkFence> fences;
    VkPipelineLayout pipelineLayout;
    VkCommandPool primaryCommandPool;
    std::vector<ImageInfo> images;
    int lastDepthPrepassFrameIndex = 0;
    std::vector<VkImageView> imageViews;
    GraphicsPipelineInfo graphicsPipeline;
    std::vector<VkSemaphore> waitSemaphores;
    std::vector<VkFramebuffer> frameBuffers;
    DepthPrepassShaderUniform::Vertex vsData;
    VkDescriptorSetLayout descriptorSetLayout;
    std::vector<VkDescriptorSet> descriptorSets;
    std::vector<VkSemaphore> finishedSemaphores;
    std::vector<VkPipelineStageFlags> waitStages;
    std::vector<VkDescriptorPool> descriptorPools;
    std::vector<VkCommandBuffer> primaryCommandBuffers;
    std::vector<UniformBufferPack> vertexShaderUniformBuffers;
    std::vector<std::vector<DescriptorBinding>> descriptorBindings;
    std::vector<std::vector<VkDescriptorBufferInfo>> vertexShaderDescriptorBuffers;
} ;
