#pragma once
#include "../../Includes.h"
#include "../DescriptorBinding/Data/DescriptorBinding.h"



class DescriptorPool{
    
public:
    
    static VkDescriptorPool Build(VkDevice logicalDevice, const std::vector<DescriptorBinding>& descriptorBindings, uint32_t numSets);
    static VkDescriptorPool Build2(VkDevice logicalDevice, const std::vector<std::vector<DescriptorBinding>>& descriptorBindings);
    static void Reset(VkDevice logicalDevice, VkDescriptorPool descriptorPool, VkDescriptorPoolResetFlags flags);
    static void Destroy(VkDevice logicalDevice, VkDescriptorPool descriptorPool);
};
