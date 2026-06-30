#include "DepthPrepass.h"




void DepthPrepass::Build(ContextInfo& context, DepthPrepassInfo& depthPrepass)
{
    depthPrepass.shader = Core::Shader::Build(context.logicalDevice, depthPrepassShaderConfig);
    
    depthPrepass.vertexInput = VertexInput::Build(depthPrepassVSFilePath);
    
    depthPrepass.renderPass = RenderPass::BuildRenderPass(RenderPassBuildConfiguration{
        .logicalDevice = context.logicalDevice,
        .subpassDependencyBuildConfigurations = std::vector<SubpassDependencyBuildConfiguration>{
            SubpassDependencyBuildConfiguration{
                .srcSubpass = VK_SUBPASS_EXTERNAL,
                .dstSubpass = 0,
                .srcStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
                .srcAccessMask = 0,
                .dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
                .dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT
                /*
                .srcSubpass = VK_SUBPASS_EXTERNAL,
                .dstSubpass = 0,
                .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
                .srcAccessMask = 0,
                .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
                .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT*/
            }
        },
            .attachmentDescriptionBuildConfigurations = std::vector<AttachmentDescriptionBuildConfiguration>{
                AttachmentDescriptionBuildConfiguration{
                    .format = context.depthFormat,
                    .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
                    .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
                    .samples = VK_SAMPLE_COUNT_1_BIT,
                    .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                    .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
                    .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                    .finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                }
            },
            .subpassDescriptionBuildConfigurations = std::vector<SubpassDescriptionBuildConfiguration>{
                SubpassDescriptionBuildConfiguration{
                    .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
                    .depthAttachmentReference = VkAttachmentReference{
                        .attachment = 0,
                        .layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
                    },
                        .colorAttachmentReferences = {}
                }
            }
    });
    
    
    depthPrepass.descriptorSetLayout = DescriptorSetLayout::Create(context.logicalDevice, depthPrepassDescriptorBindings);
    
    depthPrepass.pipelineLayout = PipelineLayout::Build(context.logicalDevice, {depthPrepass.descriptorSetLayout});
    
    depthPrepass.graphicsPipeline = GraphicsPipeline::Build(context.logicalDevice, depthPrepassGraphicsPipelineConfig, depthPrepass.renderPass, depthPrepass.pipelineLayout, depthPrepass.shader, depthPrepass.vertexInput);
    
    size_t n = context.swapChain.imageCount;
    depthPrepass.images.resize(n);
    for(int i = 0; i < n; i++)
        depthPrepass.images[i] = Image::BuildRawImage( {
            .logicalDevice = context.logicalDevice,
            .physicalDevice = context.physicalDevicePack.physicalDevice,
            .resolution = context.swapChain.extent,
            .depth = 1,
            .format = context.depthFormat,
            .tiling = VK_IMAGE_TILING_OPTIMAL,
            .usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
            .properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            .mipLevels = 1,
            .arrayLayers = 1,
            .imageFlag = 0,
            .imageType = VK_IMAGE_TYPE_2D,
            .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
            .samples = VK_SAMPLE_COUNT_1_BIT,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE
        });

    depthPrepass.imageViews.resize(n) ;
    for(uint32_t i = 0; i < n; i++)
        depthPrepass.imageViews[i] = ImageView::Build(ImageViewBuildInfo{
            .logicalDevice = context.logicalDevice,
            .format = context.depthFormat,
            .aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT,
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .components = {
                .r = VK_COMPONENT_SWIZZLE_IDENTITY,
                .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                .a = VK_COMPONENT_SWIZZLE_IDENTITY
            },
            .image = depthPrepass.images[i].handle,
            .levelCount = 1,
            .layerCount = 1,
            .baseMipLevel = 0,
            .baseArrayLayer = 0
        });
      
    depthPrepass.sampler = Sampler::Build(context.logicalDevice, context.physicalDevicePack.physicalDevice, depthPrepassSamplerConfig);
    
    depthPrepass.frameBuffers.resize(n) ;
    for(size_t i = 0; i < n; i++)
        depthPrepass.frameBuffers[i] = FrameBuffer::Create(context.logicalDevice, depthPrepass.renderPass, context.swapChain.extent, 1, {depthPrepass.imageViews[i]});
    
    depthPrepass.primaryCommandPool = CommandPool::Create(context.logicalDevice, context.physicalDevicePack.queueIndex, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
    
    depthPrepass.primaryCommandBuffers = CommandBuffer::Allocate(context.logicalDevice, depthPrepass.primaryCommandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, MAX_FRAMES_IN_FLIGHT);
            
    depthPrepass.fences.resize(MAX_FRAMES_IN_FLIGHT) ;
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        depthPrepass.fences[i] = Fence::Create(context.logicalDevice, VK_FENCE_CREATE_SIGNALED_BIT);
        
    depthPrepass.finishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        depthPrepass.finishedSemaphores[i] = Semaphore::Build(context.logicalDevice);
    
    depthPrepass.descriptorPools.resize(MAX_FRAMES_IN_FLIGHT);
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        depthPrepass.descriptorPools[i] = DescriptorPool::Build(context.logicalDevice, depthPrepassDescriptorBindings, MAX_FRAMES_IN_FLIGHT);
            
    depthPrepass.vertexShaderUniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    for(uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        depthPrepass.vertexShaderUniformBuffers[i] = Buffer::BuildUniformBuffer(BuildUniformBufferConfiguration{
            .logicalDevice = context.logicalDevice,
            .physicalDevice = context.physicalDevicePack.physicalDevice,
            .bufferSize = sizeof(DepthPrepassShaderUniform::Vertex)
        });
    
    depthPrepass.vertexShaderDescriptorBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    for(uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++){
        depthPrepass.vertexShaderDescriptorBuffers[i].resize(1);
        depthPrepass.vertexShaderDescriptorBuffers[i][0].buffer = depthPrepass.vertexShaderUniformBuffers[i].bufferPack.buffer;
        depthPrepass.vertexShaderDescriptorBuffers[i][0].offset = 0;
        depthPrepass.vertexShaderDescriptorBuffers[i][0].range = sizeof(DepthPrepassShaderUniform::Vertex);
    }
    
    depthPrepass.descriptorBindings.assign(MAX_FRAMES_IN_FLIGHT, depthPrepassDescriptorBindings);
    for(uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        depthPrepass.descriptorBindings[i][0].descriptorBuffers = depthPrepass.vertexShaderDescriptorBuffers[i];
    
    depthPrepass.descriptorSets = DescriptorSet::Build(context.logicalDevice, depthPrepass.descriptorSetLayout, depthPrepass.descriptorPools[0], depthPrepass.descriptorBindings);
}

void DepthPrepass::Load(ContextInfo& context, DepthPrepassInfo& depthPrepass, ModelPack& modelPack)
{
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        depthPrepass.descriptorBindings[i][1].descriptorBuffers = modelPack.modelMatricesDescriptorBuffers[i];
    
    UpdateDescriptorSets(context, depthPrepass);
}

void DepthPrepass::UpdateDescriptorSets(ContextInfo& context, DepthPrepassInfo& depthPrepass)
{
    DescriptorPool::Reset(context.logicalDevice, depthPrepass.descriptorPools[0], 0);
    
    depthPrepass.descriptorSets = DescriptorSet::Build(context.logicalDevice, depthPrepass.descriptorSetLayout, depthPrepass.descriptorPools[0], depthPrepass.descriptorBindings);
}

void DepthPrepass::HandleWindowResize(ContextInfo& context, DepthPrepassInfo& depthPrepass)
{
    size_t n = context.swapChain.imageCount;
    
    for(int i = 0; i < n; i++){
        Image::Destroy(context.logicalDevice, depthPrepass.images[i]);
        depthPrepass.images[i] = Image::BuildRawImage( {
            .logicalDevice = context.logicalDevice,
            .physicalDevice = context.physicalDevicePack.physicalDevice,
            .resolution = context.swapChain.extent,
            .depth = 1,
            .format = context.depthFormat,
            .tiling = VK_IMAGE_TILING_OPTIMAL,
            .usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
            .properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            .mipLevels = 1,
            .arrayLayers = 1,
            .imageFlag = 0,
            .imageType = VK_IMAGE_TYPE_2D,
            .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
            .samples = VK_SAMPLE_COUNT_1_BIT,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE
        });
    }

    for(uint32_t i = 0; i < n; i++){
        ImageView::Destroy(context.logicalDevice, depthPrepass.imageViews[i]);
        depthPrepass.imageViews[i] = ImageView::Build(ImageViewBuildInfo{
            .logicalDevice = context.logicalDevice,
            .format = context.depthFormat,
            .aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT,
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .components = {
                .r = VK_COMPONENT_SWIZZLE_IDENTITY,
                .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                .a = VK_COMPONENT_SWIZZLE_IDENTITY
            },
            .image = depthPrepass.images[i].handle,
            .levelCount = 1,
            .layerCount = 1,
            .baseMipLevel = 0,
            .baseArrayLayer = 0
        });
    }
    
    for(size_t i = 0; i < n; i++){
        FrameBuffer::Destroy(context.logicalDevice, depthPrepass.frameBuffers[i]);
        depthPrepass.frameBuffers[i] = FrameBuffer::Create(context.logicalDevice, depthPrepass.renderPass, context.swapChain.extent, 1, {depthPrepass.imageViews[i]});
    }
}

void DepthPrepass::Record(ContextInfo& context, DepthPrepassInfo& depthPrepass, OcclusionCullingInfo& occlusionCulling, ModelPack& modelPack, CameraPack& cameraPack, uint32_t frame)
{
    CommandBuffer::Reset(depthPrepass.primaryCommandBuffers[frame]);
    
    CommandBuffer::Begin(depthPrepass.primaryCommandBuffers[frame], 0, nullptr);
    
    PipelineBarrier::InsertImageMemoryBarrier(InsertImageMemoryBarrierConfiguration{
        .graphicsQueue = context.queuePack.graphicsQueue,
        .logicalDevice = context.logicalDevice,
        .commandPool = depthPrepass.primaryCommandPool,
        .dependencyFlags = 0,
        .commandBuffer = depthPrepass.primaryCommandBuffers[frame],
        .imageMemoryBarrierConfigurations = std::vector<ImageMemoryBarrierConfiguration>{
            ImageMemoryBarrierConfiguration{
                .image = depthPrepass.images[frame].handle,
                .levelCount = 1,
                .layerCount = 1,
                .baseMipLevel = 0,
                .baseArrayLayer = 0,
                .oldLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL,
                .newLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                .aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT,
                .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED
            }
        }
    });

    RenderPass::BeginRenderPass(RenderPassBeginConfiguration{
        .commandBuffer = depthPrepass.primaryCommandBuffers[frame],
        .frameBuffer = depthPrepass.frameBuffers[frame],
        .resolution = context.swapChain.extent,
        .renderPass = depthPrepass.renderPass,
        .clearValues = std::vector<VkClearValue>{
            { .depthStencil = {1.0f, 0} }
        }
    });
    
    Viewport::Set(depthPrepass.primaryCommandBuffers[frame], 0, std::vector<VkViewport>{
        VkViewport{
            .x = 0.0f,
            .y = 0.0f,
            .width = static_cast<float>(context.swapChain.extent.width),
            .height = static_cast<float>(context.swapChain.extent.height),
            .minDepth = 0.0f,
            .maxDepth = 1.0f
        }
    });
    
    Scissor::SetScissor(SetScissorConfiguration{
        .offset = 0,
        .commandBuffer = depthPrepass.primaryCommandBuffers[frame],
        .scissors = std::vector<VkRect2D>{
            VkRect2D{
                .offset = { 0, 0 },
                .extent = context.swapChain.extent
            }
        }
    });
    
    GraphicsPipeline::Bind(depthPrepass.primaryCommandBuffers[frame], depthPrepass.graphicsPipeline.handle) ;
        
    Buffer::CopyMemory(CopyMemoryConfiguration{
        .data = &depthPrepass.vsData,
        .dataSize = sizeof(DepthPrepassShaderUniform::Vertex),
        .memoryPointer = depthPrepass.vertexShaderUniformBuffers[frame].memoryPointer
    });
    
    Buffer::CopyMemory(CopyMemoryConfiguration{
        .data = modelPack.modelMatrices.data(),
        .dataSize = sizeof(modelPack.modelMatrices[0]) * modelPack.modelMatrices.size(),
        .memoryPointer = modelPack.modelMatricesBuffers[frame].memoryPointer
    });
    
    DescriptorSet::Bind(depthPrepass.primaryCommandBuffers[frame], VK_PIPELINE_BIND_POINT_GRAPHICS, depthPrepass.pipelineLayout, {depthPrepass.descriptorSets[frame]});
    
    Buffer::BindVertexBuffer(BindVertexBufferConfiguration{
        .offsets = {0},
        .firstBinding = 0,
        .commandBuffer = depthPrepass.primaryCommandBuffers[frame],
        .vertexBuffers = {modelPack.vertexBuffer.buffer}
    }) ;
   
    Buffer::BindIndexBuffer(BindIndexBufferConfiguration{
        .commandBuffer = depthPrepass.primaryCommandBuffers[frame],
        .indexBuffer = modelPack.indexBuffer.buffer,
        .type = VK_INDEX_TYPE_UINT32,
        .offset = 0
    }) ;
    
    Drawer::DrawIndexedIndirect(depthPrepass.primaryCommandBuffers[frame], occlusionCulling.outputDrawCommandsBuffers[frame].buffer, 0, modelPack.drawCommands.size(), sizeof(VkDrawIndexedIndirectCommand));
    //Drawer::DrawIndexedIndirect(depthPrepass.primaryCommandBuffers[frame], frustumCulling.visibleDrawCommandsBuffers[frame].buffer, 0, modelPack.drawCommands.size(), sizeof(VkDrawIndexedIndirectCommand));
    
    RenderPass::EndRenderPass(depthPrepass.primaryCommandBuffers[frame]);

    PipelineBarrier::InsertImageMemoryBarrier(InsertImageMemoryBarrierConfiguration{
        .graphicsQueue = context.queuePack.graphicsQueue,
        .logicalDevice = context.logicalDevice,
        .commandPool = depthPrepass.primaryCommandPool,
        .dependencyFlags = 0,
        .commandBuffer = depthPrepass.primaryCommandBuffers[frame],
        .imageMemoryBarrierConfigurations = std::vector<ImageMemoryBarrierConfiguration>{
            ImageMemoryBarrierConfiguration{
                .image = depthPrepass.images[frame].handle,
                .levelCount = 1,
                .layerCount = 1,
                .baseMipLevel = 0,
                .baseArrayLayer = 0,
                .oldLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                .newLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL,
                .aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT,
                .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED
            }
        }
    });
    
    CommandBuffer::End(depthPrepass.primaryCommandBuffers[frame]);
}

void DepthPrepass::Submit(DepthPrepassInfo& depthPrepass, OcclusionCullingInfo& occlusionCulling, ContextInfo& context, uint32_t frame)
{
    Queue::SubmitQueue(SubmitQueueConfiguration{
        .fence = depthPrepass.fences[frame],
        .graphicsQueue = context.queuePack.graphicsQueue,
        .submissionInfos = std::vector<SubmitQueueInfo>{
            SubmitQueueInfo{
                .waitSemaphores = {occlusionCulling.finishedSemaphores[frame]},
                .signalSemaphores = {depthPrepass.finishedSemaphores[frame]},
                .commandBuffers = {depthPrepass.primaryCommandBuffers[frame]},
                .waitStages = {VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT}
            }
        }
    });
}
