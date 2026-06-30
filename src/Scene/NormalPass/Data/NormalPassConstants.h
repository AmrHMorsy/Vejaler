#pragma once
#include "NormalPassShaderUniform.h"
#include "../../../Core/DescriptorBinding/Data/DescriptorBinding.h"




const std::string vertexShaderFilePath = "../Resources/Shaders/NormalPass/vs.spv";

const std::string fragmentShaderFilePath = "../Resources/Shaders/NormalPass/fs.spv";

const std::vector<DescriptorBinding> descriptorBindingSchematic = std::vector<DescriptorBinding>{
    DescriptorBinding{
        .binding = 0,
        .descriptorActualCount = 1,
        .descriptorMaxCapacity = 1,
        .stageFlag = VK_SHADER_STAGE_VERTEX_BIT,
        .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .descriptorImages = {VkDescriptorImageInfo{}},
        .descriptorBuffers = {VkDescriptorBufferInfo{}},
        .bufferSize = sizeof(NormalPassShaderUniform::Vertex)
    },
    DescriptorBinding{
        .binding = 1,
        .descriptorActualCount = 1,
        .descriptorMaxCapacity = 1,
        .stageFlag = VK_SHADER_STAGE_FRAGMENT_BIT,
        .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .descriptorImages = {VkDescriptorImageInfo{}},
        .descriptorBuffers = {VkDescriptorBufferInfo{}},
        .bufferSize = sizeof(NormalPassShaderUniform::Fragment)
    },
    DescriptorBinding{
        .binding = 2,
        .descriptorActualCount = 1,
        .descriptorMaxCapacity = 1,
        .stageFlag = VK_SHADER_STAGE_FRAGMENT_BIT,
        .descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER,
        .descriptorImages = {VkDescriptorImageInfo{}},
        .descriptorBuffers = {VkDescriptorBufferInfo{}},
        .bufferSize = 0,
        .descriptorBindingFlags = 0
    },
    DescriptorBinding{
        .binding = 3,
        .descriptorActualCount = 1,
        .descriptorMaxCapacity = 1,
        .stageFlag = VK_SHADER_STAGE_VERTEX_BIT,
        .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
        .descriptorImages = {VkDescriptorImageInfo{}},
        .descriptorBuffers = {VkDescriptorBufferInfo{}}
    },
    DescriptorBinding{
        .binding = 4,
        .descriptorMaxCapacity = 60,
        .stageFlag = VK_SHADER_STAGE_FRAGMENT_BIT,
        .descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
        .descriptorImages = {VkDescriptorImageInfo{}},
        .descriptorBuffers = {VkDescriptorBufferInfo{}},
        .descriptorBindingFlags = VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT | VK_DESCRIPTOR_BINDING_VARIABLE_DESCRIPTOR_COUNT_BIT
    }
};
