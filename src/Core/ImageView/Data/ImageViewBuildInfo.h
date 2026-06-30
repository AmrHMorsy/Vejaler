#pragma once
#include "../../../Includes.h"



struct ImageViewBuildInfo{
    VkImage image ;
    VkFormat format ;
    uint32_t levelCount ;
    uint32_t layerCount ;
    uint32_t baseMipLevel ;
    VkDevice logicalDevice ;
    uint32_t baseArrayLayer ;
    VkImageViewType viewType ;
    VkComponentMapping components;
    VkImageAspectFlags aspectMask;
} ;
