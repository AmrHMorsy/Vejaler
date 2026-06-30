#pragma once
#include "../../../Includes.h"


#define DEPTH_PYRAMID_DESCRIPTOR_BINDING_INDEX              0
#define UNIFORM_DESCRIPTOR_BINDING_INDEX                    1
#define MODEL_MATRICES_DESCRIPTOR_BINDING_INDEX             2
#define MAX_SCALES_DESCRIPTOR_BINDING_INDEX                 3
#define BOUNDING_SPHERES_DESCRIPTOR_BINDING_INDEX           4
#define INPUT_DRAW_COMMANDS_DESCRIPTOR_BINDING_INDEX        5
#define OUTPUT_DRAW_COMMANDS_DESCRIPTOR_BINDING_INDEX       6
#define OUTPUT_DRAW_COMMANDS_COUNT_DESCRIPTOR_BINDING_INDEX 7




const std::string occlusionCullingComputeShaderFilePath = "../Resources/Shaders/OcclusionCulling/cs.spv";

const std::vector<DescriptorBinding> occlusionCullingDescriptorBindingsSchematic = {
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
        .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
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
        .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
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
    },
    DescriptorBinding{
        .binding = 7,
        .descriptorActualCount = 1,
        .descriptorMaxCapacity = 1,
        .stageFlag = VK_SHADER_STAGE_COMPUTE_BIT,
        .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
        .descriptorImages = {VkDescriptorImageInfo{}},
        .descriptorBuffers = {VkDescriptorBufferInfo{}}
    }
};
