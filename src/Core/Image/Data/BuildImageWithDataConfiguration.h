#pragma once
#include "../../../Includes.h"



struct BuildImageWithDataConfiguration{
    void * imageData;
    uint32_t arrayLayers;
    VkQueue graphicsQueue;
    VkDeviceSize imageSize;
    VkDevice logicalDevice;
    VkFormat textureFormat;
    VkCommandPool commandPool;
    VkExtent2D imageResolution;
    uint32_t imageNumMipLevels;
    VkImageCreateFlags imageFlag;
    VkPhysicalDevice physicalDevice;
    VkImageLayout intermediateLayout;
};
