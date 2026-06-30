#pragma once
#include "../../Includes.h"



class PipelineLayout{
    
public:
    
    static VkPipelineLayout Build(VkDevice logicalDevice, std::vector<VkDescriptorSetLayout> descriptorSetLayouts);
    static void Destroy(VkDevice logicalDevice, VkPipelineLayout& pipelineLayout) ;
};
