#include "Drawer.h"




void Drawer::Draw(VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
{
    vkCmdDraw(commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance) ;
}

void Drawer::DrawIndexed(VkCommandBuffer commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, uint32_t vertexOffset, uint32_t firstInstance)
{
    vkCmdDrawIndexed(commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}

void Drawer::DrawIndexedIndirect(VkCommandBuffer commandBuffer, VkBuffer drawCommandsBuffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride)
{
    if((drawCommandsBuffer == VK_NULL_HANDLE) || (drawCount == 0))
        return;
    
    vkCmdDrawIndexedIndirect(commandBuffer, drawCommandsBuffer, offset, drawCount, stride);
}
