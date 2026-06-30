#pragma once
#include "../../../Includes.h"



struct PhysicalDevicePack{
    uint32_t queueIndex ;
    VkPhysicalDevice physicalDevice ;
    VkSurfaceCapabilitiesKHR capabilities ;
    std::vector<VkSurfaceFormatKHR> formats ;
    std::vector<VkPresentModeKHR> presentModes ;
    int maxPerStageDescriptorSampledImages ;
} ;
