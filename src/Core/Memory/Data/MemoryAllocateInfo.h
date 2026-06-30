#pragma once
#include "../../../Includes.h"


struct MemoryAllocateInfo{
    VkDevice logicalDevice ;
    VkPhysicalDevice physicalDevice ;
    VkMemoryPropertyFlags properties ;
    VkMemoryRequirements memRequirements ;
} ;
