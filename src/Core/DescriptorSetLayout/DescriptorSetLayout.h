#pragma once
#include "../../Includes.h"
#include "../DescriptorBinding/Data/DescriptorBinding.h"




class DescriptorSetLayout {
    
private:
    
    static std::vector<VkDescriptorSetLayoutBinding> GetDescriptorSetLayoutBindings(const std::vector<DescriptorBinding>& descriptorBindings, uint32_t numBindings);
    
    static std::vector<VkDescriptorBindingFlags> GetDescriptorBindingFlags(const std::vector<DescriptorBinding>& descriptorBindings, uint32_t numBindings);
    
public:
    
    static VkDescriptorSetLayout Create(VkDevice logicalDevice, const std::vector<DescriptorBinding>& descriptorBindings);
};
