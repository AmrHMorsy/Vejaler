#pragma once
#include "../../Includes.h"



class FrameBuffer{
    
public:
    
    static VkFramebuffer Create(VkDevice logicalDevice, VkRenderPass renderPass, VkExtent2D resolution, uint32_t layers, const std::vector<VkImageView>& imageViews);
    static void Destroy(VkDevice logicalDevice, VkFramebuffer frameBuffer);
};
