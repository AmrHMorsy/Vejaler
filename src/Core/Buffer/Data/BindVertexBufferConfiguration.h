#pragma once
#include "../../../Includes.h"


struct BindVertexBufferConfiguration{
    uint32_t firstBinding;
    VkCommandBuffer commandBuffer;
    std::vector<VkDeviceSize> offsets;
    std::vector<VkBuffer> vertexBuffers;
};

