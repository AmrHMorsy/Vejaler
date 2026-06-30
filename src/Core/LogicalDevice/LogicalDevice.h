#pragma once
#include "../../Includes.h"




class LogicalDevice {
    
private:
    
    static std::vector<VkDeviceQueueCreateInfo> BuildDeviceQueue(uint32_t queueIndex);

public:
    
    static VkDevice Build(VkPhysicalDevice physicalDevice, uint32_t queueIndex);
    static void WaitIdle(VkDevice logicalDevice);
    static void Destroy(VkDevice logicalDevice);
};
