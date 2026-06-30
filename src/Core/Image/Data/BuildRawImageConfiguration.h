#pragma once
#include "../../../Includes.h"


struct BuildRawImageConfiguration{
    uint32_t depth ;
    VkFormat format ;
    uint32_t mipLevels ;
    uint32_t arrayLayers ;
    VkImageTiling tiling ;
    VkImageType imageType ;
    VkExtent2D resolution ;
    VkDevice logicalDevice ;
    VkImageUsageFlags usage ;
    VkSharingMode sharingMode ;
    VkImageLayout initialLayout ;
    VkImageCreateFlags imageFlag ;
    VkSampleCountFlagBits samples ;
    VkPhysicalDevice physicalDevice ;
    VkMemoryPropertyFlags properties ;
} ;
