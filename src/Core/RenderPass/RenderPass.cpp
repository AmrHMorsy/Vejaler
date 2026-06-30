#include "RenderPass.h"



VkRenderPass RenderPass::BuildRenderPass(RenderPassBuildConfiguration configuration)
{
    RenderPassBuildStorage storage;
    FillRenderPassStorage(configuration, storage);
    
    VkRenderPassCreateInfo info{};
    FillRenderPassCreateInfo(storage, info);
    
    VkRenderPass renderPass ;
    VkResult result = vkCreateRenderPass(configuration.logicalDevice, &info, nullptr, &renderPass);
    
    if(result != VK_SUCCESS)
        throw std::runtime_error("\n\033[31mERROR::FAILURE TO CREATE RENDER PASS\033[0m");
    
    return renderPass ;
}

void RenderPass::BeginRenderPass(RenderPassBeginConfiguration configuration)
{
    VkRenderPassBeginInfo info{
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        .renderPass = configuration.renderPass,
        .framebuffer = configuration.frameBuffer,
        .renderArea.offset = {0, 0},
        .renderArea.extent = configuration.resolution,
        .clearValueCount = static_cast<uint32_t>(configuration.clearValues.size()),
        .pClearValues = configuration.clearValues.data()
    } ;
    vkCmdBeginRenderPass(configuration.commandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE) ;
}

void RenderPass::EndRenderPass(VkCommandBuffer commandBuffer)
{
    vkCmdEndRenderPass(commandBuffer) ;
}

void RenderPass::DestroyRenderPass(RenderPassDestroyConfiguration configuration)
{
    vkDestroyRenderPass(configuration.logicalDevice, configuration.renderPass, NULL) ;
}

void RenderPass::FillRenderPassStorage(const RenderPassBuildConfiguration& configuration, RenderPassBuildStorage& storage)
{
    SubpassDependency::BuildSubpassDependencies(configuration.subpassDependencyBuildConfigurations, storage.subpassDependencies);

    AttachmentDescription::BuildAttachmentDescriptions(configuration.attachmentDescriptionBuildConfigurations, storage.attachmentDescriptions);

    SubpassDescription::BuildSubpassDescriptions(configuration.subpassDescriptionBuildConfigurations, storage.subpassDescriptions) ;
}

void RenderPass::FillRenderPassCreateInfo(const RenderPassBuildStorage& storage, VkRenderPassCreateInfo& info)
{
    info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO ;
    info.attachmentCount = static_cast<uint32_t>(storage.attachmentDescriptions.size());
    info.pAttachments = storage.attachmentDescriptions.data() ;
    info.pSubpasses = storage.subpassDescriptions.data();
    info.subpassCount = static_cast<uint32_t>(storage.subpassDescriptions.size());
    info.pDependencies = storage.subpassDependencies.data();
    info.dependencyCount = static_cast<uint32_t>(storage.subpassDependencies.size());
}
