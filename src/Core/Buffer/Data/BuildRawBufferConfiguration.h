#pragma once
#include "../../../Includes.h"



struct BuildRawBufferConfiguration{
    VkDeviceSize size ;
    VkDevice logicalDevice ;
    VkBufferUsageFlags usage ;
    VkSharingMode sharingMode ;
    VkPhysicalDevice physicalDevice ;
    VkMemoryPropertyFlags properties ;
} ;
