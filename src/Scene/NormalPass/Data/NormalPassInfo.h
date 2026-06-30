#pragma once
#include "../../../Includes.h"
#include "NormalPassShaderUniform.h"
#include "../../../Core/Image/Data/ImageInfo.h"
#include "../../../Core/Shader/Data/ShaderInfo.h"
#include "../../../Core/Buffer/Data/UniformBufferPack.h"
#include "../../../Core/VertexInput/Data/VertexInputInfo.h"
#include "../../../Core/GraphicsPipeline/Data/GraphicsPipelineInfo.h"
#include "../../../Core/GraphicsPipeline/Data/GraphicsPipelineConfig.h"
#include "../../../Core/DescriptorBinding/Data/DescriptorBinding.h"




struct NormalPassInfo{
    ShaderInfo shader;
    VkRenderPass renderPass;
    std::vector<VkFence> fences;
    VertexInputInfo vertexInput;
    std::vector<ImageInfo> images;
    VkPipelineLayout pipelineLayout;
    VkDescriptorPool descriptorPool;
    std::vector<std::thread> threads;
    VkCommandPool primaryCommandPool;
    std::vector<VkImageView> imageViews;
    GraphicsPipelineInfo graphicsPipeline;
    NormalPassShaderUniform::Vertex vsData;
    std::vector<VkFramebuffer> frameBuffers;
    NormalPassShaderUniform::Fragment fsData;
    VkDescriptorSetLayout descriptorSetLayout;
    std::vector<VkDescriptorSet> descriptorSets;
    std::vector<VkSemaphore> finishedSemaphores;
    GraphicsPipelineConfig graphicsPipelineConfig;
    std::vector<VkCommandBuffer> primaryCommandBuffers;
    std::vector<UniformBufferPack> vertexShaderUniformBuffers;
    std::vector<UniformBufferPack> fragmentShaderUniformBuffers;
    std::vector<std::vector<DescriptorBinding>> descriptorBindings;
};
