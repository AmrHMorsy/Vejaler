#include "Texture.h"



void Texture::BuildWithImage(ContextInfo& context, TexturePack& texture, const TextureConfig& config)
{
    texture.config = config;
        
    texture.sourceImage = SourceImage::LoadLDR(texture.config.filePath, texture.config.resolution);

    VkCommandPool commandPool = CommandPool::Create(context.logicalDevice, context.physicalDevicePack.queueIndex, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
    
    uint32_t numMipLevels = (texture.config.mipMapped)? texture.sourceImage.numMipLevels: 1;

    texture.image = Image::BuildImageWithData(BuildImageWithDataConfiguration{
        .intermediateLayout = texture.config.intermediateLayout,
        .imageNumMipLevels = numMipLevels,
        .arrayLayers = texture.config.arrayLayers,
        .textureFormat = texture.config.textureFormat,
        .imageFlag = texture.config.imageFlag,
        .commandPool = commandPool,
        .graphicsQueue = context.queuePack.graphicsQueue,
        .logicalDevice = context.logicalDevice,
        .physicalDevice = context.physicalDevicePack.physicalDevice,
        .imageSize = texture.sourceImage.imageSize,
        .imageResolution = texture.sourceImage.resolution,
        .imageData = (void*)texture.sourceImage.image
    });
    
    if(texture.config.mipMapped){
        GenerateMipmaps(MipMapsGenerateConfiguration{
            .logicalDevice = context.logicalDevice,
            .physicalDevice = context.physicalDevicePack.physicalDevice,
            .commandPool = commandPool,
            .graphicsQueue = context.queuePack.graphicsQueue,
            .image = texture.image.handle,
            .imageResolution = texture.sourceImage.resolution,
            .numMipLevels = numMipLevels,
            .arrayLayers = texture.config.arrayLayers,
            .resolutionStrings = texture.resolutionStrings
        });
        size_t n = texture.resolutionStrings.size();
        texture.resolutionStringsPtr.resize(n);
        for(size_t i = 0; i < n; i++)
            texture.resolutionStringsPtr[i] = texture.resolutionStrings[i].c_str();
    }
    
    PipelineBarrier::InsertImageMemoryBarrier(InsertImageMemoryBarrierConfiguration{
        .logicalDevice = context.logicalDevice,
        .commandPool = commandPool,
        .graphicsQueue = context.queuePack.graphicsQueue,
        .dependencyFlags = 0,
        .imageMemoryBarrierConfigurations = std::vector<ImageMemoryBarrierConfiguration>{
            ImageMemoryBarrierConfiguration{
                .oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                .newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                .image = texture.image.handle,
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .baseMipLevel = 0,
                .levelCount = numMipLevels,
                .baseArrayLayer = 0,
                .layerCount = 1
            }
        },
    });

    texture.imageView = ImageView::Build(ImageViewBuildInfo{
        .logicalDevice = context.logicalDevice,
        .viewType = texture.config.viewType,
        .format = texture.config.textureFormat,
        .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
        .components = {
            .r = VK_COMPONENT_SWIZZLE_IDENTITY,
            .g = VK_COMPONENT_SWIZZLE_IDENTITY,
            .b = VK_COMPONENT_SWIZZLE_IDENTITY,
            .a = VK_COMPONENT_SWIZZLE_IDENTITY
        },
        .levelCount = numMipLevels,
        .layerCount = texture.config.arrayLayers,
        .baseMipLevel = 0,
        .baseArrayLayer = 0,
        .image = texture.image.handle
    });
    
    
    SamplerConfig samplerConfig = SamplerConfig{
        .magFilter = VK_FILTER_LINEAR,
        .minFilter = VK_FILTER_LINEAR,
        .addressMode = texture.config.addressMode,
        .anisotropyEnable = VK_FALSE,
        .borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE,
        .unnormalizedCoordinates = VK_FALSE,
        .compareEnable = VK_FALSE,
        .compareOp = VK_COMPARE_OP_ALWAYS,
        .mipMapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,
        .mipLodBias = 0.0f,
        .minLod = 0.0f,
        .maxLod = float(numMipLevels)
    };
    texture.sampler = Sampler::Build(context.logicalDevice, context.physicalDevicePack.physicalDevice, samplerConfig);
    
    texture.uiTextureID = (ImTextureID)
        ImGui_ImplVulkan_AddTexture(texture.sampler, texture.imageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    
    CommandPool::Destroy(context.logicalDevice, commandPool);
}

void Texture::BuildWithImage(ContextInfo& context, TexturePack& texture, SourceImageInfo& sourceImage, const TextureConfig& config)
{
    texture.config = config;
    
    texture.sourceImage = sourceImage;
    
    VkCommandPool commandPool = CommandPool::Create(context.logicalDevice, context.physicalDevicePack.queueIndex, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
    
    uint32_t numMipLevels = (texture.config.mipMapped)? texture.sourceImage.numMipLevels: 1;

    texture.image = Image::BuildImageWithData(BuildImageWithDataConfiguration{
        .intermediateLayout = texture.config.intermediateLayout,
        .imageNumMipLevels = numMipLevels,
        .arrayLayers = texture.config.arrayLayers,
        .textureFormat = texture.config.textureFormat,
        .imageFlag = texture.config.imageFlag,
        .commandPool = commandPool,
        .graphicsQueue = context.queuePack.graphicsQueue,
        .logicalDevice = context.logicalDevice,
        .physicalDevice = context.physicalDevicePack.physicalDevice,
        .imageSize = texture.sourceImage.imageSize,
        .imageResolution = texture.sourceImage.resolution,
        .imageData = (void*)texture.sourceImage.image
    });
    
    if(texture.config.mipMapped){
        GenerateMipmaps(MipMapsGenerateConfiguration{
            .logicalDevice = context.logicalDevice,
            .physicalDevice = context.physicalDevicePack.physicalDevice,
            .commandPool = commandPool,
            .graphicsQueue = context.queuePack.graphicsQueue,
            .image = texture.image.handle,
            .imageResolution = texture.sourceImage.resolution,
            .numMipLevels = numMipLevels,
            .arrayLayers = texture.config.arrayLayers,
            .resolutionStrings = texture.resolutionStrings
        });
        size_t n = texture.resolutionStrings.size();
        texture.resolutionStringsPtr.resize(n);
        for(size_t i = 0; i < n; i++)
            texture.resolutionStringsPtr[i] = texture.resolutionStrings[i].c_str();
    }
    
    PipelineBarrier::InsertImageMemoryBarrier(InsertImageMemoryBarrierConfiguration{
        .logicalDevice = context.logicalDevice,
        .commandPool = commandPool,
        .graphicsQueue = context.queuePack.graphicsQueue,
        .dependencyFlags = 0,
        .imageMemoryBarrierConfigurations = std::vector<ImageMemoryBarrierConfiguration>{
            ImageMemoryBarrierConfiguration{
                .oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                .newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                .image = texture.image.handle,
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .baseMipLevel = 0,
                .levelCount = numMipLevels,
                .baseArrayLayer = 0,
                .layerCount = 1
            }
        },
    });

    texture.imageView = ImageView::Build(ImageViewBuildInfo{
        .logicalDevice = context.logicalDevice,
        .viewType = texture.config.viewType,
        .format = texture.config.textureFormat,
        .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
        .components = {
            .r = VK_COMPONENT_SWIZZLE_IDENTITY,
            .g = VK_COMPONENT_SWIZZLE_IDENTITY,
            .b = VK_COMPONENT_SWIZZLE_IDENTITY,
            .a = VK_COMPONENT_SWIZZLE_IDENTITY
        },
        .levelCount = numMipLevels,
        .layerCount = texture.config.arrayLayers,
        .baseMipLevel = 0,
        .baseArrayLayer = 0,
        .image = texture.image.handle
    });
    
    
    SamplerConfig samplerConfig = SamplerConfig{
        .magFilter = VK_FILTER_LINEAR,
        .minFilter = VK_FILTER_LINEAR,
        .addressMode = texture.config.addressMode,
        .anisotropyEnable = VK_FALSE,
        .borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE,
        .unnormalizedCoordinates = VK_FALSE,
        .compareEnable = VK_FALSE,
        .compareOp = VK_COMPARE_OP_ALWAYS,
        .mipMapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,
        .mipLodBias = 0.0f,
        .minLod = 0.0f,
        .maxLod = float(numMipLevels)
    };
    texture.sampler = Sampler::Build(context.logicalDevice, context.physicalDevicePack.physicalDevice, samplerConfig);
    
    texture.uiTextureID = (ImTextureID)
        ImGui_ImplVulkan_AddTexture(texture.sampler, texture.imageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    
    CommandPool::Destroy(context.logicalDevice, commandPool);
}

void Texture::BuildTextureWithImage(ContextInfo& context, TexturePack& texture)
{
    VkCommandPool commandPool = CommandPool::Create(context.logicalDevice, context.physicalDevicePack.queueIndex, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
    
    uint32_t numMipLevels = (texture.config.mipMapped)? texture.sourceImage.numMipLevels: 1;

    texture.image = Image::BuildImageWithData(BuildImageWithDataConfiguration{
        .intermediateLayout = texture.config.intermediateLayout,
        .imageNumMipLevels = numMipLevels,
        .arrayLayers = texture.config.arrayLayers,
        .textureFormat = texture.config.textureFormat,
        .imageFlag = texture.config.imageFlag,
        .commandPool = commandPool,
        .graphicsQueue = context.queuePack.graphicsQueue,
        .logicalDevice = context.logicalDevice,
        .physicalDevice = context.physicalDevicePack.physicalDevice,
        .imageSize = texture.sourceImage.imageSize,
        .imageResolution = texture.sourceImage.resolution,
        .imageData = (void*)texture.sourceImage.image
    });
    
    if(texture.config.mipMapped){
        GenerateMipmaps(MipMapsGenerateConfiguration{
            .logicalDevice = context.logicalDevice,
            .physicalDevice = context.physicalDevicePack.physicalDevice,
            .commandPool = commandPool,
            .graphicsQueue = context.queuePack.graphicsQueue,
            .image = texture.image.handle,
            .imageResolution = texture.sourceImage.resolution,
            .numMipLevels = numMipLevels,
            .arrayLayers = texture.config.arrayLayers,
            .resolutionStrings = texture.resolutionStrings
        });
        size_t n = texture.resolutionStrings.size();
        texture.resolutionStringsPtr.resize(n);
        for(size_t i = 0; i < n; i++)
            texture.resolutionStringsPtr[i] = texture.resolutionStrings[i].c_str();
    }
        
    PipelineBarrier::InsertImageMemoryBarrier(InsertImageMemoryBarrierConfiguration{
        .logicalDevice = context.logicalDevice,
        .commandPool = commandPool,
        .graphicsQueue = context.queuePack.graphicsQueue,
        .dependencyFlags = 0,
        .imageMemoryBarrierConfigurations = std::vector<ImageMemoryBarrierConfiguration>{
            ImageMemoryBarrierConfiguration{
                .oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                .newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                .image = texture.image.handle,
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .baseMipLevel = 0,
                .levelCount = numMipLevels,
                .baseArrayLayer = 0,
                .layerCount = 1
            }
        },
    });

    texture.imageView = ImageView::Build(ImageViewBuildInfo{
        .logicalDevice = context.logicalDevice,
        .viewType = texture.config.viewType,
        .format = texture.config.textureFormat,
        .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
        .components = {
            .r = VK_COMPONENT_SWIZZLE_IDENTITY,
            .g = VK_COMPONENT_SWIZZLE_IDENTITY,
            .b = VK_COMPONENT_SWIZZLE_IDENTITY,
            .a = VK_COMPONENT_SWIZZLE_IDENTITY
        },
        .levelCount = numMipLevels,
        .layerCount = texture.config.arrayLayers,
        .baseMipLevel = 0,
        .baseArrayLayer = 0,
        .image = texture.image.handle
    });
    
    
    SamplerConfig samplerConfig = SamplerConfig{
        .magFilter = VK_FILTER_LINEAR,
        .minFilter = VK_FILTER_LINEAR,
        .addressMode = texture.config.addressMode,
        .anisotropyEnable = VK_FALSE,
        .borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE,
        .unnormalizedCoordinates = VK_FALSE,
        .compareEnable = VK_FALSE,
        .compareOp = VK_COMPARE_OP_ALWAYS,
        .mipMapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,
        .mipLodBias = 0.0f,
        .minLod = 0.0f,
        .maxLod = float(numMipLevels)
    };
    texture.sampler = Sampler::Build(context.logicalDevice, context.physicalDevicePack.physicalDevice, samplerConfig);
    
    texture.uiTextureID = (ImTextureID)
        ImGui_ImplVulkan_AddTexture(texture.sampler, texture.imageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    
    CommandPool::Destroy(context.logicalDevice, commandPool);
}


















TexturePack Texture::BuildRawTexture(RawTextureBuildConfiguration configuration)
{
    ImageInfo image = Image::BuildRawImage(BuildRawImageConfiguration{
        .logicalDevice = configuration.logicalDevice,
        .physicalDevice = configuration.physicalDevice,
        .resolution = configuration.resolution,
        .depth = 1,
        .format = configuration.textureFormat,
        .tiling = VK_IMAGE_TILING_OPTIMAL,
        .usage = configuration.imageUsageFlags,
        .properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        .mipLevels = configuration.numMipLevels,
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
                .oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                .newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                .image = image.handle,
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .baseMipLevel = 0,
                .levelCount = configuration.numMipLevels,
                .baseArrayLayer = 0,
                .layerCount = configuration.arrayLayers
            }
        }
    });

    VkImageView imageView = ImageView::Build({
        .logicalDevice = configuration.logicalDevice,
        .viewType = configuration.viewType,
        .format = configuration.textureFormat,
        .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
        .components = {
            .r = VK_COMPONENT_SWIZZLE_IDENTITY,
            .g = VK_COMPONENT_SWIZZLE_IDENTITY,
            .b = VK_COMPONENT_SWIZZLE_IDENTITY,
            .a = VK_COMPONENT_SWIZZLE_IDENTITY
        },
        .levelCount = configuration.numMipLevels,
        .layerCount = configuration.arrayLayers,
        .baseMipLevel = 0,
        .baseArrayLayer = 0,
        .image = image.handle
    }) ;

    SamplerConfig samplerConfig = SamplerConfig{
        .magFilter = VK_FILTER_LINEAR,
        .minFilter = VK_FILTER_LINEAR,
        .addressMode = configuration.addressMode,
        .anisotropyEnable = configuration.anisotropyEnable,
        .borderColor = configuration.borderColor,
        .unnormalizedCoordinates = VK_FALSE,
        .compareEnable = VK_FALSE,
        .compareOp = VK_COMPARE_OP_ALWAYS,
        .mipMapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,
        .mipLodBias = 0.0f,
        .minLod = 0.0f,
        .maxLod = float(configuration.numMipLevels)
    };
    VkSampler sampler = Sampler::Build(configuration.logicalDevice, configuration.physicalDevice, samplerConfig);

    return TexturePack{
        .sampler = sampler,
        .imageView = imageView,
        .image = image.handle
    } ;
}

void Texture::GenerateMipmaps(MipMapsGenerateConfiguration configuration)
{
    VkCommandBuffer commandBuffer = CommandBuffer::Allocate(configuration.logicalDevice, configuration.commandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1)[0];
    
    CommandBuffer::Begin(commandBuffer, 0, nullptr);
    
    configuration.resolutionStrings.resize(configuration.numMipLevels);
    
    int32_t mipWidth = configuration.imageResolution.width ;
    int32_t mipHeight = configuration.imageResolution.height ;
    
    configuration.resolutionStrings[0] = std::to_string(mipWidth) + " x " + std::to_string(mipHeight) + " -- Mip Level " + std::to_string(0);
    
    for(uint32_t i = 1; i < configuration.numMipLevels; i++){
        
        PipelineBarrier::InsertImageMemoryBarrier(InsertImageMemoryBarrierConfiguration{
            .graphicsQueue = configuration.graphicsQueue,
            .logicalDevice = configuration.logicalDevice,
            .commandPool = configuration.commandPool,
            .dependencyFlags = 0,
            .commandBuffer = commandBuffer,
            .imageMemoryBarrierConfigurations = std::vector<ImageMemoryBarrierConfiguration>{
                ImageMemoryBarrierConfiguration{
                    .image = configuration.image,
                    .levelCount = 1,
                    .layerCount = configuration.arrayLayers,
                    .baseMipLevel = i-1,
                    .baseArrayLayer = 0,
                    .oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                    .newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                    .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT
                }
            }
        });

        Image::BlitImage(ImageBlitConfiguration{
            .filter = VK_FILTER_LINEAR,
            .commandBuffer = commandBuffer,
            .srcImage = configuration.image,
            .dstImage = configuration.image,
            .graphicsQueue = configuration.graphicsQueue,
            .logicalDevice = configuration.logicalDevice,
            .commandPool = configuration.commandPool,
            .srcImageLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            .dstImageLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            .imageBlitRegionConfigurations = std::vector<ImageBlitRegionConfiguration>{
                ImageBlitRegionConfiguration{
                    .srcMipLevel = i-1,
                    .dstMipLevel = i,
                    .srcLayerCount = configuration.arrayLayers,
                    .dstLayerCount = configuration.arrayLayers,
                    .srcBaseArrayLayer = 0,
                    .dstBaseArrayLayer = 0,
                    .srcAspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                    .dstAspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                    .srcOffsets = std::array<VkOffset3D, 2>{
                        VkOffset3D{},
                        VkOffset3D{
                            .x = mipWidth,
                            .y = mipHeight,
                            .z = 1
                        }
                    },
                    .dstOffsets = std::array<VkOffset3D, 2>{
                        VkOffset3D{},
                        VkOffset3D{
                            .x = mipWidth > 1 ? mipWidth / 2 : 1,
                            .y = mipHeight > 1 ? mipHeight / 2 : 1,
                            .z = 1
                        }
                    }
                }
            },
        });

        PipelineBarrier::InsertImageMemoryBarrier(InsertImageMemoryBarrierConfiguration{
            .graphicsQueue = configuration.graphicsQueue,
            .logicalDevice = configuration.logicalDevice,
            .commandPool = configuration.commandPool,
            .dependencyFlags = 0,
            .commandBuffer = commandBuffer,
            .imageMemoryBarrierConfigurations = std::vector<ImageMemoryBarrierConfiguration>{
                ImageMemoryBarrierConfiguration{
                    .image = configuration.image,
                    .levelCount = 1,
                    .layerCount = configuration.arrayLayers,
                    .baseMipLevel = i-1,
                    .baseArrayLayer = 0,
                    .oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                    .newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                    .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT
                }
            }
        });

        mipWidth = std::max(1, mipWidth / 2);
        mipHeight = std::max(1, mipHeight / 2);
        
        configuration.resolutionStrings[i] = std::to_string(mipWidth) + " x " + std::to_string(mipHeight) + " -- Mip Level " + std::to_string(i);
    }

    PipelineBarrier::InsertImageMemoryBarrier(InsertImageMemoryBarrierConfiguration{
        .graphicsQueue = configuration.graphicsQueue,
        .logicalDevice = configuration.logicalDevice,
        .commandPool = configuration.commandPool,
        .dependencyFlags = 0,
        .commandBuffer = commandBuffer,
        .imageMemoryBarrierConfigurations = std::vector<ImageMemoryBarrierConfiguration>{
            ImageMemoryBarrierConfiguration{
                .image = configuration.image,
                .levelCount = 1,
                .layerCount = configuration.arrayLayers,
                .baseMipLevel = configuration.numMipLevels-1,
                .baseArrayLayer = 0,
                .oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                .newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT
            }
        }
    });

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
    });

    Fence::Wait(configuration.logicalDevice, {fence}, VK_TRUE, UINT64_MAX);

    Fence::Destroy(configuration.logicalDevice, fence);
}
