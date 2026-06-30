#pragma once
#include "../../Includes.h"




class Fence{
    
public:
    
    static VkFence Create(VkDevice logicalDevice, VkFenceCreateFlags flags);
    static void Wait(VkDevice logicalDevice, const std::vector<VkFence>& fences, VkBool32 waitAll, uint64_t timeout);
    static void Reset(VkDevice logicalDevice, const std::vector<VkFence>& fences);
    static void Destroy(VkDevice logicalDevice, VkFence fence);
};
