#pragma once
#include "Data/SamplerConfig.h"




class Sampler{
    
public:
    
    static VkSampler Build(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, const SamplerConfig& config);
    static void Destroy(VkDevice logicalDevice, VkSampler& sampler);
};
