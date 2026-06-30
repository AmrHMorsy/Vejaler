#pragma once
#include "SSAOShaderUniform.h"
#include "../../../Core/DescriptorBinding/Data/DescriptorBinding.h"



const std::string vertexShaderFilePath = "../Resources/Shaders/SSAO/vs.spv";

const std::string fragmentShaderFilePath = "../Resources/Shaders/SSAO/fs.spv";

const std::vector<DescriptorBinding> descriptorBindingSchematic = std::vector<DescriptorBinding>{
    DescriptorBinding{
        .binding = 0,
        .descriptorActualCount = 1,
        .descriptorMaxCapacity = 1,
        .stageFlag = VK_SHADER_STAGE_FRAGMENT_BIT,
        .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .descriptorImages = {VkDescriptorImageInfo{}},
        .descriptorBuffers = {VkDescriptorBufferInfo{}},
        .bufferSize = sizeof(SSAOShaderUniform::Fragment)
    },
    DescriptorBinding{
        .binding = 1,
        .descriptorActualCount = 1,
        .descriptorMaxCapacity = 1,
        .stageFlag = VK_SHADER_STAGE_FRAGMENT_BIT,
        .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .descriptorImages = {VkDescriptorImageInfo{}},
        .descriptorBuffers = {VkDescriptorBufferInfo{}},
        .bufferSize = 0
    },
    DescriptorBinding{
        .binding = 2,
        .descriptorActualCount = 1,
        .descriptorMaxCapacity = 1,
        .stageFlag = VK_SHADER_STAGE_FRAGMENT_BIT,
        .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .descriptorImages = {VkDescriptorImageInfo{}},
        .descriptorBuffers = {VkDescriptorBufferInfo{}},
        .bufferSize = 0
    },
    DescriptorBinding{
        .binding = 3,
        .descriptorActualCount = 1,
        .descriptorMaxCapacity = 1,
        .stageFlag = VK_SHADER_STAGE_FRAGMENT_BIT,
        .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .descriptorImages = {VkDescriptorImageInfo{}},
        .descriptorBuffers = {VkDescriptorBufferInfo{}},
        .bufferSize = 0
    }
};
