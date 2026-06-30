#pragma once
#include "../../../Includes.h"

#define MAX_NUM_MIP_LEVELS 20



const std::string depthPyramidComputeShaderFilePath = "../Resources/Shaders/DepthPyramid/cs.spv";

const std::vector<DescriptorBinding> depthPyramidDescriptorBindingsSchematic = {
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
        .descriptorMaxCapacity = MAX_NUM_MIP_LEVELS,
        .stageFlag = VK_SHADER_STAGE_COMPUTE_BIT,
        .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
        .descriptorImages = {VkDescriptorImageInfo{}},
        .descriptorBuffers = {VkDescriptorBufferInfo{}}
    },
    DescriptorBinding{
        .binding = 2,
        .descriptorActualCount = 1,
        .descriptorMaxCapacity = 1,
        .stageFlag = VK_SHADER_STAGE_COMPUTE_BIT,
        .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .descriptorImages = {VkDescriptorImageInfo{}},
        .descriptorBuffers = {VkDescriptorBufferInfo{}}
    }
};
