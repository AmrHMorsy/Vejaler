#pragma once
#include "../../../Includes.h"



struct MapMemoryConfiguration{
    VkDeviceSize offset;
    VkDeviceSize dataSize;
    VkDevice logicalDevice;
    VkDeviceMemory bufferMemory;
    VkMemoryMapFlags memoryMapFlags;
};
