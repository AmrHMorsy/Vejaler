#include "Image.h"



ImageInfo Image::BuildRawImage(BuildRawImageConfiguration configuration)
{
    VkImageCreateInfo imageCreateInfo{
        .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .imageType = configuration.imageType,
        .extent.width = configuration.resolution.width,
        .extent.height = configuration.resolution.height,
        .extent.depth = configuration.depth,
        .mipLevels = configuration.mipLevels,
        .arrayLayers = configuration.arrayLayers,
        .format = configuration.format,
        .tiling = configuration.tiling,
        .initialLayout = configuration.initialLayout,
        .usage = configuration.usage,
        .samples = configuration.samples,
        .sharingMode = configuration.sharingMode,
        .flags = configuration.imageFlag
    };
    
    VkImage image;
    VkResult result = vkCreateImage(configuration.logicalDevice, &imageCreateInfo, nullptr, &image);
    if(result != VK_SUCCESS)
        throw std::runtime_error("\033[31m\nERROR::FAILURE TO CREATE IMAGE\033[0m");
    
    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(configuration.logicalDevice, image, &memRequirements);
    VkDeviceMemory memory = Memory::AllocateMemory(MemoryAllocateInfo{
        .logicalDevice = configuration.logicalDevice,
        .physicalDevice = configuration.physicalDevice,
        .properties = configuration.properties,
        .memRequirements = memRequirements
    });
    vkBindImageMemory(configuration.logicalDevice, image, memory, 0);
    
    return ImageInfo{
        .handle = image,
        .memory = memory
    };
}

ImageInfo Image::BuildImageWithData(BuildImageWithDataConfiguration configuration)
{
    ImageInfo image = BuildRawImage(BuildRawImageConfiguration{
        .logicalDevice = configuration.logicalDevice,
        .physicalDevice = configuration.physicalDevice,
        .resolution = configuration.imageResolution,
        .depth = 1,
        .format = configuration.textureFormat,
        .tiling = VK_IMAGE_TILING_OPTIMAL,
        .usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        .properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        .mipLevels = configuration.imageNumMipLevels,
        .arrayLayers = configuration.arrayLayers,
        .imageFlag = configuration.imageFlag,
        .imageType = VK_IMAGE_TYPE_2D,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE
    }) ;
    
    PipelineBarrier::InsertImageMemoryBarrier(InsertImageMemoryBarrierConfiguration{
        .logicalDevice = configuration.logicalDevice,
        .commandPool = configuration.commandPool,
        .graphicsQueue = configuration.graphicsQueue,
        .dependencyFlags = 0,
        .imageMemoryBarrierConfigurations = std::vector<ImageMemoryBarrierConfiguration>{
            ImageMemoryBarrierConfiguration{
                .image = image.handle,
                .baseMipLevel = 0,
                .levelCount = configuration.imageNumMipLevels,
                .baseArrayLayer = 0,
                .layerCount = configuration.arrayLayers,
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                .newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
            }
        }
    });
        
    BufferPack stageBuffer = Buffer::BuildRawBuffer(BuildRawBufferConfiguration{
        .size = configuration.imageSize,
        .logicalDevice = configuration.logicalDevice,
        .usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .physicalDevice = configuration.physicalDevice,
        .properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
    });
    
    Buffer::UploadDataToBuffer(UploadDataToBufferConfiguration{
        .offset = 0,
        .data = configuration.imageData,
        .memoryMapFlags = 0,
        .dataSize = configuration.imageSize,
        .bufferMemory = stageBuffer.bufferMemory,
        .logicalDevice = configuration.logicalDevice,
    });

    Buffer::CopyBufferToImage(CopyBufferToImageConfiguration{
        .buffer = stageBuffer.buffer,
        .image = image.handle,
        .commandPool = configuration.commandPool,
        .logicalDevice = configuration.logicalDevice,
        .graphicsQueue = configuration.graphicsQueue,
        .copyRegions = std::vector<VkBufferImageCopy>{
            VkBufferImageCopy{
                .bufferOffset = 0,
                .bufferRowLength = 0,
                .bufferImageHeight = 0,
                .imageOffset = {0, 0, 0},
                .imageSubresource.mipLevel = 0,
                .imageSubresource.layerCount = 1,
                .imageSubresource.baseArrayLayer = 0,
                .imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .imageExtent = {configuration.imageResolution.width, configuration.imageResolution.height, 1},
            }
        }
    });
    
    return image;
}

void Image::FillImageBlit(const ImageBlitRegionConfiguration& configuration, VkImageBlit& imageBlitRegion)
{
    imageBlitRegion = {};
    
    imageBlitRegion.srcOffsets[0] = configuration.srcOffsets[0];
    imageBlitRegion.srcOffsets[1] = configuration.srcOffsets[1];
    imageBlitRegion.srcSubresource.aspectMask = configuration.srcAspectMask;
    imageBlitRegion.srcSubresource.mipLevel = configuration.srcMipLevel;
    imageBlitRegion.srcSubresource.baseArrayLayer = configuration.srcBaseArrayLayer;
    imageBlitRegion.srcSubresource.layerCount = configuration.srcLayerCount;
    
    imageBlitRegion.dstOffsets[0] = configuration.dstOffsets[0];
    imageBlitRegion.dstOffsets[1] = configuration.dstOffsets[1];
    imageBlitRegion.dstSubresource.aspectMask = configuration.dstAspectMask;
    imageBlitRegion.dstSubresource.mipLevel = configuration.dstMipLevel;
    imageBlitRegion.dstSubresource.baseArrayLayer = configuration.dstBaseArrayLayer;
    imageBlitRegion.dstSubresource.layerCount = configuration.dstLayerCount;
}

void Image::BlitImage(ImageBlitConfiguration configuration)
{
    size_t n = configuration.imageBlitRegionConfigurations.size();
    
    std::vector<VkImageBlit> imageBlitRegions(n);
    for(size_t i = 0; i < n; i++ )
        FillImageBlit(configuration.imageBlitRegionConfigurations[i], imageBlitRegions[i]);
        
    if(configuration.commandBuffer != VK_NULL_HANDLE)
        vkCmdBlitImage(configuration.commandBuffer, configuration.srcImage, configuration.srcImageLayout, configuration.dstImage, configuration.dstImageLayout, static_cast<uint32_t>(imageBlitRegions.size()), imageBlitRegions.data(), configuration.filter);
    else{
        VkCommandBuffer commandBuffer = CommandBuffer::Allocate(configuration.logicalDevice, configuration.commandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1)[0];

        CommandBuffer::Begin(commandBuffer, 0, nullptr);
        
        vkCmdBlitImage(commandBuffer, configuration.srcImage, configuration.srcImageLayout, configuration.dstImage, configuration.dstImageLayout, static_cast<uint32_t>(imageBlitRegions.size()), imageBlitRegions.data(), configuration.filter);

        CommandBuffer::End(commandBuffer) ;

        VkFence fence = Fence::Create(configuration.logicalDevice, 0);
        
        Queue::SubmitQueue(SubmitQueueConfiguration{
            .fence = fence,
            .graphicsQueue = configuration.graphicsQueue,
            .submissionInfos = std::vector<SubmitQueueInfo>{
                SubmitQueueInfo{
                    .waitSemaphores = {VK_NULL_HANDLE},
                    .signalSemaphores = {VK_NULL_HANDLE},
                    .commandBuffers = {commandBuffer},
                    .waitStages = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT}
                }
            }
        }) ;
        
        Fence::Wait(configuration.logicalDevice, {fence}, VK_TRUE, UINT64_MAX);
        
        Fence::Destroy(configuration.logicalDevice, fence);
        
        CommandBuffer::Free(configuration.logicalDevice, configuration.commandPool, {commandBuffer});
    }
}

void Image::Destroy(VkDevice logicalDevice, ImageInfo image)
{
    vkDestroyImage(logicalDevice, image.handle, NULL);
    vkFreeMemory(logicalDevice, image.memory, NULL);
}
