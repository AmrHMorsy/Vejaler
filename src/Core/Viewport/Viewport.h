#pragma once
#include "../../Includes.h"




class Viewport{
    
public:
    
    static void Set(VkCommandBuffer commandBuffer, uint32_t offset, const std::vector<VkViewport>& viewports);
};
