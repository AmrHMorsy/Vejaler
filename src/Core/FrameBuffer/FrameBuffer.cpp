#include "FrameBuffer.h"




VkFramebuffer FrameBuffer::Create(VkDevice logicalDevice, VkRenderPass renderPass, VkExtent2D resolution, uint32_t layers, const std::vector<VkImageView>& imageViews)
{
    VkFramebufferCreateInfo framebufferCreateInfo{};
    
    framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferCreateInfo.renderPass = renderPass;
    framebufferCreateInfo.attachmentCount = static_cast<uint32_t>(imageViews.size());
    framebufferCreateInfo.pAttachments = imageViews.data();
    framebufferCreateInfo.width = resolution.width;
    framebufferCreateInfo.height = resolution.height;
    framebufferCreateInfo.layers = layers;
    
    VkFramebuffer frameBuffer;
    VkResult result = vkCreateFramebuffer(logicalDevice, &framebufferCreateInfo, nullptr, &frameBuffer);
    
    if(result != VK_SUCCESS)
        throw std::runtime_error("\033[31m\nERROR::FAILURE TO CREATE FRAMEBUFFERS\033[0m");

    return frameBuffer ;
}

void FrameBuffer::Destroy(VkDevice logicalDevice, VkFramebuffer frameBuffer)
{
    vkDestroyFramebuffer(logicalDevice, frameBuffer, nullptr);
}
