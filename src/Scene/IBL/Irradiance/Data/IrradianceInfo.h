#pragma once
#include "IrradianceConfig.h"
#include "../../../../Includes.h"
#include "IrradianceShaderUniform.h"
#include "../../../../Core/Core.h"
#include "../../../Texture/Data/TexturePack.h"
#include "../../../../Core/GraphicsPipeline/Data/GraphicsPipelineConfig.h"



struct IrradianceInfo{
    VkFence fence;
    ShaderInfo shader;
    TexturePack texture;
    IrradianceConfig config;
    BufferPack vertexBuffer;
    VkRenderPass renderPass;
    VertexInputInfo vertexInput;
    void * vertexShaderUniformData;
    VkPipelineLayout pipelineLayout;
    VkDescriptorPool descriptorPool;
    VkCommandPool primaryCommandPool;
    std::vector<std::thread> threads;
    size_t vertexShaderUniformDataSize;
    std::vector<VkImageView> imageViews;
    VkCommandBuffer primaryCommandBuffer;
    GraphicsPipelineInfo graphicsPipeline;
    std::vector<VkFramebuffer> frameBuffers;
    VkDescriptorSetLayout descriptorSetLayout;
    std::vector<VkDescriptorSet> descriptorSets;
    std::vector<UniformBufferPack> uniformBuffers;
    GraphicsPipelineConfig graphicsPipelineConfig;
    std::vector<VkCommandPool> secondaryCommandPools;
    std::vector<VkCommandBuffer> secondaryCommandBuffers;
    std::vector<DescriptorBinding> descriptorBindingSchematic;
    std::vector<std::vector<DescriptorBinding>> descriptorBindings;
    std::vector<IrradianceShaderUniform::Vertex> vertexShaderUniformDatas;
};
