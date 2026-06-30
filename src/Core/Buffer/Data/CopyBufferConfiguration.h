#pragma once
#include "../../../Includes.h"




struct CopyBufferConfiguration{
    VkBuffer srcBuffer;
    VkBuffer dstBuffer;
    VkQueue graphicsQueue;
    VkDevice logicalDevice;
    VkCommandPool commandPool;
    std::vector<VkBufferCopy> copyRegions;
};
