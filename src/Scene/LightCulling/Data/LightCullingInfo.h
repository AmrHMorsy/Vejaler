#pragma once
#include "../../../Includes.h"
#include "LightCullingUniformVariables.h"



struct LightCullingInfo{
    ShaderInfo shader;
    glm::uvec2 numTiles2D;
    unsigned int numLights;
    unsigned int totalNumTiles;
    VkPipeline computePipeline;
    std::vector<VkFence> fences;
    VkPipelineLayout pipelineLayout;
    VkCommandPool primaryCommandPool;
    LightCullingUniformVariables uniformData;
    std::vector<glm::vec4> tilesFrustumPlanes;
    VkDescriptorSetLayout descriptorSetLayout;
    std::vector<VkDescriptorSet> descriptorSets;
    std::vector<VkSemaphore> finishedSemaphores;
    std::vector<BufferPack> tileLightCountBuffers;
    std::vector<BufferPack> maxLightCountBuffers;
    std::vector<UniformBufferPack> uniformBuffers;
    std::vector<BufferPack> tileLightIndicesBuffers;
    VkDescriptorPool descriptorPool = VK_NULL_HANDLE;
    std::vector<VkCommandBuffer> primaryCommandBuffers;
    std::vector<glm::vec4> lightBoundingSpheresViewSpace;
    std::vector<HostVisibleBuffer> tilesFrustumPlanesBuffers;
    std::vector<std::vector<DescriptorBinding>> descriptorBindings;
    std::vector<HostVisibleBuffer> lightBoundingSphereViewSpaceBuffers;
};
