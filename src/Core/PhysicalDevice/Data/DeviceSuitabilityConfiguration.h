#pragma once
#include "../../../Includes.h"



struct DeviceSuitabilityConfiguration{
    uint32_t queueIndex;
    VkPhysicalDevice physicalDevice;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};
