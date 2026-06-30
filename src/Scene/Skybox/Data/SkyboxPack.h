#pragma once
#include "SkyboxInfo.h"




struct SkyboxPack{
    std::vector<ShaderInfo> shaders;
    std::vector<VertexInputInfo> vertexInputs;
    std::vector<VkPipelineLayout> pipelineLayouts;
    std::vector<GraphicsPipelineInfo> graphicsPipelines;
    std::vector<GraphicsPipelineConfig> graphicsPipelineConfigs;
    std::vector<VkDescriptorSetLayout> descriptorSetLayouts;
    std::vector<std::vector<DescriptorBinding>> descriptorBindingSchematics;
    
    
    int selected;
    uint32_t mipLevels;
    bool bakingInProgress;
    std::vector<std::thread> threads;
    VkRenderPass hdrToCubemapRenderPass;
    VkCommandBuffer hdrToCubemapPrimaryCommandBuffer;
    VkCommandPool hdrToCubemapPrimaryCommandPool;

    std::vector<const char *> names;
    std::vector<SkyboxInfo> skyboxes;    
    
    std::vector<VkCommandPool> secondaryCommandPools;
    std::vector<VkCommandBuffer> secondaryCommandBuffers;
};
