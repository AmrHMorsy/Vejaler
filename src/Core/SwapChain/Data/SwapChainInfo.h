#pragma once
#include "../../../Includes.h"



struct SwapChainInfo{
    VkExtent2D extent;
    uint32_t imageCount;
    VkSwapchainKHR handle;
    std::vector<VkImage> images;
    std::vector<VkImageView> imageViews;
    VkSurfaceFormatKHR swapChainSurface;
    VkPresentModeKHR swapChainPresentMode;
};
