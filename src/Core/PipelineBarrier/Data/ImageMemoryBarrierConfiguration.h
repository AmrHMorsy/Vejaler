#pragma once
#include "../../../Includes.h"



struct ImageMemoryBarrierConfiguration{
    VkImage image;
    uint32_t levelCount;
    uint32_t layerCount;
    uint32_t baseMipLevel;
    uint32_t baseArrayLayer;
    VkImageLayout oldLayout;
    VkImageLayout newLayout;
    VkImageAspectFlags aspectMask;
    uint32_t srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    uint32_t dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
};
