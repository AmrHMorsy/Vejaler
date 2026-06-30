#pragma once
#include "../../../Includes.h"



struct SourceImageInfo{
    void * image;
    int numChannels;
    uint32_t numMipLevels;
    VkExtent2D resolution;
    VkDeviceSize imageSize;
};
