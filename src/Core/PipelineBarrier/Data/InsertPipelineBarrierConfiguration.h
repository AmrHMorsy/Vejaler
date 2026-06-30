#pragma once
#include "../../../Includes.h"



struct InsertPipelineBarrierConfiguration{
    VkQueue graphicsQueue;
    VkDevice logicalDevice;
    VkCommandPool commandPool;
    VkCommandBuffer commandBuffer;
    VkPipelineStageFlags srcStageMask ;
    VkPipelineStageFlags dstStageMask ;
    VkDependencyFlags dependencyFlags ;
    const std::vector<VkMemoryBarrier> memoryBarriers ;
    const std::vector<VkImageMemoryBarrier> imageMemoryBarriers ;
    const std::vector<VkBufferMemoryBarrier> bufferMemoryBarriers ;
} ;
