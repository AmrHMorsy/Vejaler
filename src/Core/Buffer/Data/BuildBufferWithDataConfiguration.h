#pragma once
#include "../../../Includes.h"


struct BuildBufferWithDataConfiguration{
    void * data;
    VkQueue graphicsQueue;
    VkDevice logicalDevice;
    VkDeviceSize bufferSize;
    VkCommandPool commandPool;
    VkPhysicalDevice physicalDevice;
} ;
