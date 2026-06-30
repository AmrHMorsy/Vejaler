#pragma once
#include "../../Includes.h"




class CommandPool{
    
public:
    
    static VkCommandPool Create(VkDevice logicalDevice, uint32_t queueFamilyIndex, VkCommandPoolCreateFlags flags);
    static void Destroy(VkDevice logicalDevice, VkCommandPool commandPool);
};
