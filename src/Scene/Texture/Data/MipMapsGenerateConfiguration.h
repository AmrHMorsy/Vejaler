#pragma once
#include "../../../Includes.h"


struct MipMapsGenerateConfiguration{
    VkImage image;
    VkFormat imageFormat;
    uint32_t arrayLayers;
    VkQueue graphicsQueue;
    uint32_t numMipLevels;
    VkDevice logicalDevice;
    VkCommandPool commandPool;
    VkExtent2D imageResolution;
    VkPhysicalDevice physicalDevice;
    std::vector<std::string>& resolutionStrings;
};
