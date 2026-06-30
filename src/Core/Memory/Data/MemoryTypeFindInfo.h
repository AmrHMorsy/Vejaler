#pragma once
#include "../../../Includes.h"


struct MemoryTypeFindInfo{
    uint32_t typeFilter ;
    VkPhysicalDevice physicalDevice ;
    VkMemoryPropertyFlags properties ;
} ;
