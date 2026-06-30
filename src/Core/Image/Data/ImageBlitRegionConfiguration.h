#pragma once
#include "../../../Includes.h"



struct ImageBlitRegionConfiguration{
    uint32_t srcMipLevel;
    uint32_t dstMipLevel;
    uint32_t srcLayerCount;
    uint32_t dstLayerCount;
    uint32_t srcBaseArrayLayer;
    uint32_t dstBaseArrayLayer;
    VkImageAspectFlags srcAspectMask;
    VkImageAspectFlags dstAspectMask;
    std::array<VkOffset3D, 2> srcOffsets;
    std::array<VkOffset3D, 2> dstOffsets;
};

