#include "PipelineBarrier.h"



void PipelineBarrier::InsertImageMemoryBarrier(InsertImageMemoryBarrierConfiguration configuration)
{
    size_t n = configuration.imageMemoryBarrierConfigurations.size();
    
    VkPipelineStageFlags srcStageMask = 0, dstStageMask = 0;
    std::vector<VkImageMemoryBarrier> imageMemoryBarriers(n);
    for(size_t i = 0 ; i < n; i++){
        imageMemoryBarriers[i] = FillImageMemoryBarrier(configuration.imageMemoryBarrierConfigurations[i]);
        GetPipelineStageFlags(imageMemoryBarriers[i].oldLayout, imageMemoryBarriers[i].newLayout, srcStageMask, dstStageMask);
    }
    
    InsertPipelineBarrier(InsertPipelineBarrierConfiguration{
        .memoryBarriers = {},
        .bufferMemoryBarriers = {},
        .srcStageMask = srcStageMask,
        .dstStageMask = dstStageMask,
        .commandPool = configuration.commandPool,
        .imageMemoryBarriers = imageMemoryBarriers,
        .graphicsQueue = configuration.graphicsQueue,
        .logicalDevice = configuration.logicalDevice,
        .commandBuffer = configuration.commandBuffer,
        .dependencyFlags = configuration.dependencyFlags
    });
}

void PipelineBarrier::InsertPipelineBarrier(InsertPipelineBarrierConfiguration configuration)
{
    if(configuration.commandBuffer != VK_NULL_HANDLE)
        vkCmdPipelineBarrier(configuration.commandBuffer, configuration.srcStageMask, configuration.dstStageMask, configuration.dependencyFlags, static_cast<uint32_t>(configuration.memoryBarriers.size()), configuration.memoryBarriers.data(), static_cast<uint32_t>(configuration.bufferMemoryBarriers.size()), configuration.bufferMemoryBarriers.data(), static_cast<uint32_t>(configuration.imageMemoryBarriers.size()), configuration.imageMemoryBarriers.data());
    else{
        VkCommandBuffer commandBuffer = CommandBuffer::Allocate(configuration.logicalDevice, configuration.commandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1)[0];

        CommandBuffer::Begin(commandBuffer, 0, nullptr);
        
        vkCmdPipelineBarrier(commandBuffer, configuration.srcStageMask, configuration.dstStageMask, configuration.dependencyFlags, static_cast<uint32_t>(configuration.memoryBarriers.size()), configuration.memoryBarriers.data(), static_cast<uint32_t>(configuration.bufferMemoryBarriers.size()), configuration.bufferMemoryBarriers.data(), static_cast<uint32_t>(configuration.imageMemoryBarriers.size()), configuration.imageMemoryBarriers.data());

        CommandBuffer::End(commandBuffer) ;
        
        VkFence fence = Fence::Create(configuration.logicalDevice, 0);
        
        Queue::SubmitQueue({
            .fence = fence,
            .graphicsQueue = configuration.graphicsQueue,
            .submissionInfos = std::vector<SubmitQueueInfo>{
                SubmitQueueInfo{
                    .waitSemaphores = {},
                    .signalSemaphores = {},
                    .commandBuffers = {commandBuffer},
                    .waitStages = {
                        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
                    }
                }
            }
        }) ;

        Fence::Wait(configuration.logicalDevice, {fence}, VK_TRUE, UINT64_MAX);
        
        Fence::Destroy(configuration.logicalDevice, fence);

        CommandBuffer::Free(configuration.logicalDevice, configuration.commandPool, {commandBuffer});
    }
}

VkImageMemoryBarrier PipelineBarrier::FillImageMemoryBarrier(ImageMemoryBarrierConfiguration configuration)
{
    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = configuration.oldLayout;
    barrier.newLayout = configuration.newLayout;
    barrier.srcQueueFamilyIndex = configuration.srcQueueFamilyIndex;
    barrier.dstQueueFamilyIndex = configuration.dstQueueFamilyIndex;
    barrier.image = configuration.image;
    barrier.subresourceRange.aspectMask = configuration.aspectMask;
    barrier.subresourceRange.baseMipLevel = configuration.baseMipLevel;
    barrier.subresourceRange.levelCount = configuration.levelCount;
    barrier.subresourceRange.baseArrayLayer = configuration.baseArrayLayer;
    barrier.subresourceRange.layerCount = configuration.layerCount;
    
    GetAccessFlags(barrier.oldLayout, barrier.newLayout, barrier.srcAccessMask, barrier.dstAccessMask);
    
    return barrier;
}

void PipelineBarrier::GetAccessFlags(VkImageLayout oldLayout, VkImageLayout newLayout, VkAccessFlags& srcAccessMask, VkAccessFlags& dstAccessMask)
{
    if( oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL){
        srcAccessMask = 0;
        dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    }
    else if( oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL){
        srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    }
    else if(oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL){
        srcAccessMask = 0;
        dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    }
    else if(oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL){
        srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
    }
    else if(oldLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL){
        srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    }
    else if(oldLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL){
        srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
        dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    }
    else if(oldLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL &&
             newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL){
        srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
        dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL &&
             newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
    {
        srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
        dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    }
    else
        throw std::runtime_error("\033[31m\nERROR::FAILURE TO INSERT PIPELINE BARRIER::FAILURE TO GET ACCESS FLAGS::UNSUPPORTED OLD LAYOUT AND NEW LAYOUT\033[0m");
}

void PipelineBarrier::GetPipelineStageFlags(VkImageLayout oldLayout, VkImageLayout newLayout, VkPipelineStageFlags& srcStageMask, VkPipelineStageFlags& dstStageMask)
{
    if( oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL){
        srcStageMask |= VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        dstStageMask |= VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if( oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL){
        srcStageMask |= VK_PIPELINE_STAGE_TRANSFER_BIT;
        dstStageMask |= VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else if(oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL){
        srcStageMask |= VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        dstStageMask |= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    }
    else if(oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL){
        srcStageMask |= VK_PIPELINE_STAGE_TRANSFER_BIT;
        dstStageMask |= VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if(oldLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL){
        srcStageMask |= VK_PIPELINE_STAGE_TRANSFER_BIT;
        dstStageMask |= VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else if(oldLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        srcStageMask |= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dstStageMask |= VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else if(oldLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL){
        srcStageMask |= VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        dstStageMask |= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL &&
             newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL){
        srcStageMask |= VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
        dstStageMask |= VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL &&
             newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
    {
        srcStageMask |= VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        dstStageMask |= VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    }
    else
        throw std::runtime_error("\033[31m\nERROR::FAILURE TO INSERT PIPELINE BARRIER::FAILURE TO GET STAGE FLAGS::UNSUPPORTED OLD LAYOUT AND NEW LAYOUT\033[0m");
}
