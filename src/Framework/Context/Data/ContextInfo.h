#pragma once
#include "../../../Includes.h"
#include "../../../Core/Types.h"




struct ContextInfo{
    uint32_t imageIndex;
    QueuePack queuePack;
    VkFormat depthFormat;
    VkDevice logicalDevice;
    SwapChainInfo swapChain;
    bool isQueueBusy = false;
    VkInstance vulkanInstance;
    VkSurfaceKHR windowSurface;
    PhysicalDevicePack physicalDevicePack;
    std::vector<VkSemaphore> imageAvailableSemaphores;
};
