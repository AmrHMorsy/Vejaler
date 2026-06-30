#pragma once
#include "../../../Includes.h"


struct SetScissorConfiguration{
    uint32_t offset;
    VkCommandBuffer commandBuffer;
    std::vector<VkRect2D> scissors;
};
