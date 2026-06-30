#pragma once
#include "../../Includes.h"




class Drawer{

public:
    
    static void Draw(VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance);
    static void DrawIndexed(VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, uint32_t vertexOffset, uint32_t firstInstance);
    static void DrawIndexedIndirect(VkCommandBuffer commandBuffer, VkBuffer drawCommandsBuffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride);
};
