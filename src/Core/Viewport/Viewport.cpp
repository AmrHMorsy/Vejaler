#include "Viewport.h"





void Viewport::Set(VkCommandBuffer commandBuffer, uint32_t offset, const std::vector<VkViewport>& viewports)
{
    vkCmdSetViewport(commandBuffer, offset, static_cast<uint32_t>(viewports.size()), viewports.data());
}
