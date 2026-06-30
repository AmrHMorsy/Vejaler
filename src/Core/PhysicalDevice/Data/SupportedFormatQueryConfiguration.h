#pragma once
#include "../../../Includes.h"




struct SupportedFormatQueryConfiguration{
    VkImageTiling imageTiling;
    VkPhysicalDevice physicalDevice;
    VkFormatFeatureFlags formatFeatureFlags;
    const std::vector<VkFormat>& candidates;
};
