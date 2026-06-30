#pragma once
#include "ImageBlitRegionConfiguration.h"


struct ImageBlitConfiguration{
    VkFilter filter;
    VkImage srcImage;
    VkImage dstImage;
    VkQueue graphicsQueue;
    VkDevice logicalDevice;
    VkCommandPool commandPool;
    VkImageLayout srcImageLayout;
    VkImageLayout dstImageLayout;
    VkCommandBuffer commandBuffer = VK_NULL_HANDLE;
    std::vector<ImageBlitRegionConfiguration> imageBlitRegionConfigurations;
};
