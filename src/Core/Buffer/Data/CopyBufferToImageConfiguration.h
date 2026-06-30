#pragma once
#include "../../../Includes.h"



struct CopyBufferToImageConfiguration{
    VkImage image;
    VkBuffer buffer;
    VkQueue graphicsQueue;
    VkDevice logicalDevice;
    VkCommandPool commandPool;
    std::vector<VkBufferImageCopy> copyRegions;
};
