#pragma once
#include "../../../Includes.h"



struct DescriptorBinding{
    uint32_t binding;
    size_t bufferSize;
    VkShaderStageFlags stageFlag;
    uint32_t descriptorActualCount;
    uint32_t descriptorMaxCapacity;
    VkDescriptorType descriptorType;
    VkDescriptorBindingFlags descriptorBindingFlags = 0;
    std::vector<VkDescriptorImageInfo> descriptorImages;
    std::vector<VkDescriptorBufferInfo> descriptorBuffers;
};
