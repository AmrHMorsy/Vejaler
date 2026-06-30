#pragma once
#include "../../../Includes.h"


struct SamplerConfig{
    float minLod;
    float maxLod;
    float mipLodBias;
    VkFilter magFilter;
    VkFilter minFilter;
    VkCompareOp compareOp;
    uint32_t compareEnable;
    uint32_t anisotropyEnable;
    VkBorderColor borderColor;
    VkSamplerMipmapMode mipMapMode;
    VkSamplerAddressMode addressMode;
    uint32_t unnormalizedCoordinates;
};
