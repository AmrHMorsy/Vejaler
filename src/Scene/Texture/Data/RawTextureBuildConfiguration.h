#pragma once
#include "../../../Includes.h"



struct RawTextureBuildConfiguration{
    uint32_t arrayLayers;
    uint32_t numMipLevels;
    VkExtent2D resolution;
    VkQueue graphicsQueue;
    VkDevice logicalDevice;
    VkFormat textureFormat;
    VkImageViewType viewType;
    VkCommandPool commandPool;
    VkImageLayout finalLayout;
    VkImageCreateFlags imageFlag;
    VkPhysicalDevice physicalDevice;
    VkImageLayout intermediateLayout;
    VkSamplerAddressMode addressMode;
    
    uint32_t anisotropyEnable = VK_FALSE;
    VkBorderColor borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
    VkImageUsageFlags imageUsageFlags = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
};
