#pragma once
#include "../../Includes.h"




class CommandBuffer{
    
public:
    
    static std::vector<VkCommandBuffer> Allocate(VkDevice logicalDevice, VkCommandPool commandPool, VkCommandBufferLevel level, uint32_t numCommandBuffers);
    static void Begin(VkCommandBuffer commandBuffer, VkCommandBufferUsageFlags flag, const VkCommandBufferInheritanceInfo * pInheritanceInfo);
    static void End(VkCommandBuffer commandBuffer);
    static void Reset(VkCommandBuffer commandBuffer);
    static void Free(VkDevice logicalDevice, VkCommandPool commandPool, std::vector<VkCommandBuffer> commandBuffers);
    static void Execute(VkCommandBuffer primaryCommandBuffer, std::vector<VkCommandBuffer> secondaryCommandBuffers);
};
