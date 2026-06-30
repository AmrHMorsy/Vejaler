#pragma once
#include "../../../Includes.h"




struct GetDeviceQueueConfiguration{
    uint32_t queueIndex;
    VkDevice logicalDevice;
    uint32_t queueFamilyIndex;
};
