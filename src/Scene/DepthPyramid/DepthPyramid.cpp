#include "DepthPyramid.h"





void DepthPyramid::Build(ContextInfo& context, DepthPyramidInfo& depthPyramid, DepthPrepassInfo& depthPrepass)
{
    depthPyramid.numMipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(context.swapChain.extent.width, context.swapChain.extent.height)))) + 1;
    
    depthPyramid.uniformData.numMipLevels = depthPyramid.numMipLevels;
    
    depthPyramid.numWorkGroups = glm::uvec2(std::ceil(context.swapChain.extent.width/16.0), std::ceil(context.swapChain.extent.height/16.0));
    
    depthPyramid.shader = Core::Shader::Build(context.logicalDevice, ShaderConfig{
        .filePaths = {depthPyramidComputeShaderFilePath},
        .stageFlags = {VK_SHADER_STAGE_COMPUTE_BIT}
    });
    
    depthPyramid.descriptorSetLayout = DescriptorSetLayout::Create(context.logicalDevice, depthPyramidDescriptorBindingsSchematic);
    
    depthPyramid.pipelineLayout = PipelineLayout::Build(context.logicalDevice, {depthPyramid.descriptorSetLayout});
    
    depthPyramid.computePipeline = ComputePipeline::Create(context.logicalDevice, depthPyramid.pipelineLayout, depthPyramid.shader.pipelineShaderStages[0], 1)[0];
    
    depthPyramid.primaryCommandPool = CommandPool::Create(context.logicalDevice, context.physicalDevicePack.queueIndex, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
    
    depthPyramid.primaryCommandBuffers = CommandBuffer::Allocate(context.logicalDevice, depthPyramid.primaryCommandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, MAX_FRAMES_IN_FLIGHT);
    
    depthPyramid.fences.resize(MAX_FRAMES_IN_FLIGHT) ;
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        depthPyramid.fences[i] = Fence::Create(context.logicalDevice, VK_FENCE_CREATE_SIGNALED_BIT);
    
    depthPyramid.finishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        depthPyramid.finishedSemaphores[i] = Semaphore::Build(context.logicalDevice);
    
    depthPyramid.images.resize(MAX_FRAMES_IN_FLIGHT);
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        depthPyramid.images[i] = Image::BuildRawImage({
            .logicalDevice = context.logicalDevice,
            .physicalDevice = context.physicalDevicePack.physicalDevice,
            .resolution = context.swapChain.extent,
            .depth = 1,
            .format = VK_FORMAT_R32_SFLOAT,
            .tiling = VK_IMAGE_TILING_OPTIMAL,
            .usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_STORAGE_BIT,
            .properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            .mipLevels = depthPyramid.numMipLevels,
            .arrayLayers = 1,
            .imageFlag = 0,
            .imageType = VK_IMAGE_TYPE_2D,
            .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
            .samples = VK_SAMPLE_COUNT_1_BIT,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE
        });
    
    depthPyramid.mipImageViews.resize(MAX_FRAMES_IN_FLIGHT);
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++){
        depthPyramid.mipImageViews[i].resize(depthPyramid.numMipLevels);
        for(uint32_t mip = 0; mip < depthPyramid.numMipLevels; mip++)
            depthPyramid.mipImageViews[i][mip] = ImageView::Build(ImageViewBuildInfo{
                .logicalDevice = context.logicalDevice,
                .format = VK_FORMAT_R32_SFLOAT,
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .viewType = VK_IMAGE_VIEW_TYPE_2D,
                .components = {
                    .r = VK_COMPONENT_SWIZZLE_IDENTITY,
                    .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                    .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                    .a = VK_COMPONENT_SWIZZLE_IDENTITY
                },
                .image = depthPyramid.images[i].handle,
                .levelCount = 1,
                .layerCount = 1,
                .baseMipLevel = mip,
                .baseArrayLayer = 0
            });
    }
    
    depthPyramid.fullImageViews.resize(MAX_FRAMES_IN_FLIGHT);
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        depthPyramid.fullImageViews[i] = ImageView::Build({
            .logicalDevice = context.logicalDevice,
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .format = VK_FORMAT_R32_SFLOAT,
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .components = {
                .r = VK_COMPONENT_SWIZZLE_IDENTITY,
                .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                .a = VK_COMPONENT_SWIZZLE_IDENTITY
            },
            .levelCount = depthPyramid.numMipLevels,
            .layerCount = 1,
            .baseMipLevel = 0,
            .baseArrayLayer = 0,
            .image = depthPyramid.images[i].handle
        });
    
    depthPyramid.sampler = Sampler::Build(context.logicalDevice, context.physicalDevicePack.physicalDevice, SamplerConfig{
        .magFilter = VK_FILTER_LINEAR,
        .minFilter = VK_FILTER_LINEAR,
        .addressMode = VK_SAMPLER_ADDRESS_MODE_REPEAT,
        .anisotropyEnable = VK_TRUE,
        .borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
        .unnormalizedCoordinates = VK_FALSE,
        .compareEnable = VK_FALSE,
        .compareOp = VK_COMPARE_OP_ALWAYS,
        .mipMapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,
        .mipLodBias = 0.0f,
        .minLod = 0.0f,
        .maxLod = float(depthPyramid.numMipLevels)
    });
    
    depthPyramid.descriptorBindings.assign(MAX_FRAMES_IN_FLIGHT, depthPyramidDescriptorBindingsSchematic);
    
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        depthPyramid.descriptorBindings[i][0].descriptorImages = {VkDescriptorImageInfo{
            .sampler = depthPrepass.sampler,
            .imageView = depthPrepass.imageViews[i],
            .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        }};
    
    depthPyramid.mipDescriptorImages.resize(MAX_FRAMES_IN_FLIGHT);
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++){
        depthPyramid.mipDescriptorImages[i].resize(depthPyramid.numMipLevels);
        for(uint32_t mip = 0; mip < depthPyramid.numMipLevels; mip++)
            depthPyramid.mipDescriptorImages[i][mip] = {VkDescriptorImageInfo{
                .sampler = VK_NULL_HANDLE,
                .imageView = depthPyramid.mipImageViews[i][mip],
                .imageLayout = VK_IMAGE_LAYOUT_GENERAL
            }};
    }
    
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++){
        depthPyramid.descriptorBindings[i][1].descriptorImages = depthPyramid.mipDescriptorImages[i];
        depthPyramid.descriptorBindings[i][1].descriptorActualCount = depthPyramid.numMipLevels;
    }
    
    depthPyramid.uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        depthPyramid.uniformBuffers[i] = Buffer::BuildUniformBuffer(BuildUniformBufferConfiguration{
            .logicalDevice = context.logicalDevice,
            .physicalDevice = context.physicalDevicePack.physicalDevice,
            .bufferSize = sizeof(DepthPyramidShaderUniform)
        });
    
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        depthPyramid.descriptorBindings[i][2].descriptorBuffers = {VkDescriptorBufferInfo{
            .buffer = depthPyramid.uniformBuffers[i].bufferPack.buffer,
            .offset = 0,
            .range = sizeof(DepthPyramidShaderUniform)
        }};
        
    depthPyramid.descriptorPool = DescriptorPool::Build(context.logicalDevice, depthPyramidDescriptorBindingsSchematic, MAX_FRAMES_IN_FLIGHT);
        
    depthPyramid.descriptorSets = DescriptorSet::Build(context.logicalDevice, depthPyramid.descriptorSetLayout, depthPyramid.descriptorPool, depthPyramid.descriptorBindings);
}

void DepthPyramid::Record(ContextInfo& context, DepthPyramidInfo& depthPyramid, uint32_t frame)
{
    Fence::Wait(context.logicalDevice, {depthPyramid.fences[frame]}, VK_TRUE, UINT64_MAX);
    
    Fence::Reset(context.logicalDevice, {depthPyramid.fences[frame]});
    
    CommandBuffer::Reset(depthPyramid.primaryCommandBuffers[frame]);
    
    CommandBuffer::Begin(depthPyramid.primaryCommandBuffers[frame], 0, nullptr);
    
    ComputePipeline::Bind(depthPyramid.primaryCommandBuffers[frame], depthPyramid.computePipeline);
     
    Buffer::CopyMemory(CopyMemoryConfiguration{
        .data = &depthPyramid.uniformData,
        .dataSize = sizeof(DepthPyramidShaderUniform),
        .memoryPointer = depthPyramid.uniformBuffers[frame].memoryPointer
    });
    
    DescriptorSet::Bind(depthPyramid.primaryCommandBuffers[frame], VK_PIPELINE_BIND_POINT_COMPUTE, depthPyramid.pipelineLayout, {depthPyramid.descriptorSets[frame]});
    
    vkCmdDispatch(depthPyramid.primaryCommandBuffers[frame], depthPyramid.numWorkGroups.x, depthPyramid.numWorkGroups.y, 1);
    
    CommandBuffer::End(depthPyramid.primaryCommandBuffers[frame]);
}

void DepthPyramid::Submit(ContextInfo& context, DepthPyramidInfo& depthPyramid, FrustumCullingInfo& frustumCulling, uint32_t frame)
{
    Queue::SubmitQueue(SubmitQueueConfiguration{
        .fence = depthPyramid.fences[frame],
        .graphicsQueue = context.queuePack.graphicsQueue,
        .submissionInfos = std::vector<SubmitQueueInfo>{
            SubmitQueueInfo{
                .waitSemaphores = {frustumCulling.finishedSemaphores[frame]},
                .signalSemaphores = {depthPyramid.finishedSemaphores[frame]},
                .commandBuffers = {depthPyramid.primaryCommandBuffers[frame]},
                .waitStages = {VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT}
            }
        }
    });
}

void DepthPyramid::HandleWindowResize(ContextInfo& context, DepthPrepassInfo& depthPrepass, DepthPyramidInfo& depthPyramid)
{
    depthPyramid.numMipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(context.swapChain.extent.width, context.swapChain.extent.height)))) + 1;
    
    depthPyramid.uniformData.numMipLevels = depthPyramid.numMipLevels;
    
    depthPyramid.numWorkGroups = glm::uvec2(std::ceil((context.swapChain.extent.width / 2.0f)/16.0), std::ceil((context.swapChain.extent.height / 2.0f)/16.0));
}
