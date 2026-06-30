#pragma once
#include "../../../Includes.h"


struct UploadDataToBufferConfiguration{
    void * data;
    VkDeviceSize offset;
    VkDeviceSize dataSize;
    VkDevice logicalDevice;
    VkDeviceMemory bufferMemory;
    VkMemoryMapFlags memoryMapFlags;
};
