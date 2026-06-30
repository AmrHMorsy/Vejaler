#pragma once


const uint32_t numCellsPerTile = 32;

const glm::uvec2 tileResolution(16, 16);

const std::string lightCullingComputeShaderFilePath = "../Resources/Shaders/LightCulling/LightCulling.spv";

const std::vector<DescriptorBinding> lightCullingDescriptorBindingsSchematic = {
    DescriptorBinding{
        .binding = 0,
        .descriptorActualCount = 1,
        .descriptorMaxCapacity = 1,
        .stageFlag = VK_SHADER_STAGE_COMPUTE_BIT,
        .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .descriptorImages = {VkDescriptorImageInfo{}},
        .descriptorBuffers = {VkDescriptorBufferInfo{}}
    },
    DescriptorBinding{
        .binding = 1,
        .descriptorActualCount = 1,
        .descriptorMaxCapacity = 1,
        .stageFlag = VK_SHADER_STAGE_COMPUTE_BIT,
        .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
        .descriptorImages = {VkDescriptorImageInfo{}},
        .descriptorBuffers = {VkDescriptorBufferInfo{}}
    },
    DescriptorBinding{
        .binding = 2,
        .descriptorActualCount = 1,
        .descriptorMaxCapacity = 1,
        .stageFlag = VK_SHADER_STAGE_COMPUTE_BIT,
        .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
        .descriptorImages = {VkDescriptorImageInfo{}},
        .descriptorBuffers = {VkDescriptorBufferInfo{}}
    },
    DescriptorBinding{
        .binding = 3,
        .descriptorActualCount = 1,
        .descriptorMaxCapacity = 1,
        .stageFlag = VK_SHADER_STAGE_COMPUTE_BIT,
        .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
        .descriptorImages = {VkDescriptorImageInfo{}},
        .descriptorBuffers = {VkDescriptorBufferInfo{}}
    },
    DescriptorBinding{
        .binding = 4,
        .descriptorActualCount = 1,
        .descriptorMaxCapacity = 1,
        .stageFlag = VK_SHADER_STAGE_COMPUTE_BIT,
        .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .descriptorImages = {VkDescriptorImageInfo{}},
        .descriptorBuffers = {VkDescriptorBufferInfo{}}
    },
    DescriptorBinding{
        .binding = 5,
        .descriptorActualCount = 1,
        .descriptorMaxCapacity = 1,
        .stageFlag = VK_SHADER_STAGE_COMPUTE_BIT,
        .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
        .descriptorImages = {VkDescriptorImageInfo{}},
        .descriptorBuffers = {VkDescriptorBufferInfo{}}
    },
    DescriptorBinding{
        .binding = 6,
        .descriptorActualCount = 1,
        .descriptorMaxCapacity = 1,
        .stageFlag = VK_SHADER_STAGE_COMPUTE_BIT,
        .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
        .descriptorImages = {VkDescriptorImageInfo{}},
        .descriptorBuffers = {VkDescriptorBufferInfo{}}
    }
};
