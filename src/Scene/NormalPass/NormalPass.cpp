#include "NormalPass.h"
#include "Data/NormalPassConstants.h"





void NormalPass::Build(ContextInfo& context, NormalPassInfo& normalPass, DepthPrepassInfo& depthPrepass, ModelPack& modelPack)
{
    normalPass.shader = Core::Shader::Build(context.logicalDevice, ShaderConfig{
        .filePaths = {
            vertexShaderFilePath,
            fragmentShaderFilePath
        },
        .stageFlags = {
            VK_SHADER_STAGE_VERTEX_BIT,
            VK_SHADER_STAGE_FRAGMENT_BIT
        }
    });
    
    normalPass.vertexInput = VertexInput::Build(vertexShaderFilePath);
            
    normalPass.renderPass = RenderPass::BuildRenderPass(RenderPassBuildConfiguration{
        .logicalDevice = context.logicalDevice,
        .subpassDependencyBuildConfigurations = std::vector<SubpassDependencyBuildConfiguration>{
            SubpassDependencyBuildConfiguration{
                .srcSubpass = VK_SUBPASS_EXTERNAL,
                .dstSubpass = 0,
                .srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                .srcAccessMask = VK_ACCESS_SHADER_READ_BIT,
                .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
                .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT
            }
        },
        .attachmentDescriptionBuildConfigurations = std::vector<AttachmentDescriptionBuildConfiguration>{
            AttachmentDescriptionBuildConfiguration{
                .format = VK_FORMAT_R16G16B16A16_SFLOAT,
                .samples = VK_SAMPLE_COUNT_1_BIT,
                .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
                .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
                .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
                .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                .finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
            },
            AttachmentDescriptionBuildConfiguration{
                .format = context.depthFormat,
                .loadOp = VK_ATTACHMENT_LOAD_OP_LOAD,
                .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
                .samples = VK_SAMPLE_COUNT_1_BIT,
                .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
                .initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                .finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
            }
        },
        .subpassDescriptionBuildConfigurations = std::vector<SubpassDescriptionBuildConfiguration>{
            SubpassDescriptionBuildConfiguration{
                .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
                .depthAttachmentReference = VkAttachmentReference{
                    .attachment = 1,
                    .layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
                },
                .colorAttachmentReferences = std::vector<VkAttachmentReference>{
                    VkAttachmentReference{
                        .attachment = 0,
                        .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
                    }
                }
            }
        }
    });
        
    normalPass.descriptorSetLayout = DescriptorSetLayout::Create(context.logicalDevice, descriptorBindingSchematic);

    normalPass.pipelineLayout = PipelineLayout::Build(context.logicalDevice, {normalPass.descriptorSetLayout});

    normalPass.graphicsPipelineConfig = GraphicsPipelineConfig{
        .dynamicStates = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR
        },
        .viewportStateConfig = {
            .scissorCount = 1,
            .viewportCount = 1
        },
        .rasterizerStateConfig = {
            .lineWidth = 1.0f,
            .depthBiasClamp = 0.0f,
            .frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,
            .depthBiasEnable = VK_FALSE,
            .cullMode = VK_CULL_MODE_BACK_BIT,
            .depthClampEnable = VK_FALSE,
            .polygonMode = VK_POLYGON_MODE_FILL,
            .depthBiasSlopeFactor = 0.0f,
            .depthBiasConstantFactor = 0.0f,
            .rasterizerDiscardEnable = VK_FALSE
        },
        .colorBlendStateConfig = {
            .logicOp = VK_LOGIC_OP_COPY,
            .logicOpEnable = VK_FALSE,
            .blendConstants = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
            .colorBlendAttachmentConfigs = std::vector<GraphicsPipelineConfig::ColorBlendStateConfig::ColorBlendAttachmentConfig>{
                GraphicsPipelineConfig::ColorBlendStateConfig::ColorBlendAttachmentConfig{
                    .blendEnable = VK_FALSE,
                    .srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
                    .dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
                    .colorBlendOp = VK_BLEND_OP_ADD,
                    .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
                    .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
                    .alphaBlendOp = VK_BLEND_OP_ADD,
                    .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
                }
            }
        },
        .multisampleStateConfig = {
            .minSampleShading = 1.0f,
            .alphaToOneEnable = VK_FALSE,
            .sampleShadingEnable = VK_FALSE,
            .alphaToCoverageEnable = VK_FALSE,
            .pSampleMask = nullptr,
            .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT
        },
        .depthStencilStateConfig = {
            .depthTestEnable = VK_TRUE,
            .depthWriteEnable = VK_FALSE,
            .depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL,
            .stencilTestEnable = VK_FALSE,
            .depthBoundsTestEnable = VK_FALSE
        },
        .inputAssemblyStateConfig = {
            .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
            .primitiveRestartEnable = VK_FALSE
        }
    };

    normalPass.graphicsPipeline = GraphicsPipeline::Build(context.logicalDevice, normalPass.graphicsPipelineConfig, normalPass.renderPass, normalPass.pipelineLayout, normalPass.shader, normalPass.vertexInput);
    
    normalPass.primaryCommandPool = CommandPool::Create(context.logicalDevice, context.physicalDevicePack.queueIndex, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
    
    normalPass.primaryCommandBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        normalPass.primaryCommandBuffers[i] = CommandBuffer::Allocate(context.logicalDevice, normalPass.primaryCommandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1)[0];
        
    normalPass.images.resize(MAX_FRAMES_IN_FLIGHT);
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        normalPass.images[i] = Image::BuildRawImage(BuildRawImageConfiguration{
            .logicalDevice = context.logicalDevice,
            .physicalDevice = context.physicalDevicePack.physicalDevice,
            .resolution = context.swapChain.extent,
            .depth = 1,
            .format = VK_FORMAT_R16G16B16A16_SFLOAT,
            .tiling = VK_IMAGE_TILING_OPTIMAL,
            .usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
            .properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            .mipLevels = 1,
            .arrayLayers = 1,
            .imageFlag = 0,
            .imageType = VK_IMAGE_TYPE_2D,
            .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
            .samples = VK_SAMPLE_COUNT_1_BIT,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE
        });
    
    normalPass.imageViews.resize(MAX_FRAMES_IN_FLIGHT);
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++){
        normalPass.imageViews[i] = ImageView::Build({
            .logicalDevice = context.logicalDevice,
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .format = VK_FORMAT_R16G16B16A16_SFLOAT,
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .components = {
                .r = VK_COMPONENT_SWIZZLE_IDENTITY,
                .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                .a = VK_COMPONENT_SWIZZLE_IDENTITY
            },
                .levelCount = 1,
                .layerCount = 1,
                .baseMipLevel = 0,
                .baseArrayLayer = 0,
                .image = normalPass.images[i].handle
        });
        modelPack.descriptorBindings[i][3][0].descriptorImages = {VkDescriptorImageInfo{
            .sampler = VK_NULL_HANDLE,
            .imageView = normalPass.imageViews[i],
            .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        }};
    }
    
    normalPass.frameBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        normalPass.frameBuffers[i] = FrameBuffer::Create(context.logicalDevice, normalPass.renderPass, context.swapChain.extent, 1, {normalPass.imageViews[i], depthPrepass.imageViews[i]});
        
    normalPass.fences.resize(MAX_FRAMES_IN_FLIGHT) ;
    for( int i = 0 ; i < MAX_FRAMES_IN_FLIGHT ; i++ )
        normalPass.fences[i] = Fence::Create(context.logicalDevice, VK_FENCE_CREATE_SIGNALED_BIT);
    
    normalPass.finishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    for( int i = 0 ; i < MAX_FRAMES_IN_FLIGHT ; i++ )
        normalPass.finishedSemaphores[i] = Semaphore::Build(context.logicalDevice);
    
    normalPass.descriptorPool = DescriptorPool::Build(context.logicalDevice, descriptorBindingSchematic, MAX_FRAMES_IN_FLIGHT);
    
    normalPass.descriptorBindings.assign(MAX_FRAMES_IN_FLIGHT, descriptorBindingSchematic);
    
    normalPass.vertexShaderUniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++){
        normalPass.vertexShaderUniformBuffers[i] = Buffer::BuildUniformBuffer(BuildUniformBufferConfiguration{
            .logicalDevice = context.logicalDevice,
            .physicalDevice = context.physicalDevicePack.physicalDevice,
            .bufferSize = sizeof(NormalPassShaderUniform::Vertex)
        });
        normalPass.descriptorBindings[i][0].descriptorBuffers = std::vector<VkDescriptorBufferInfo>{
            VkDescriptorBufferInfo{
                .buffer = normalPass.vertexShaderUniformBuffers[i].bufferPack.buffer,
                .offset = 0,
                .range = sizeof(NormalPassShaderUniform::Vertex)
            }
        };
    }
    
    normalPass.fragmentShaderUniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++){
        normalPass.fragmentShaderUniformBuffers[i] = Buffer::BuildUniformBuffer(BuildUniformBufferConfiguration{
            .logicalDevice = context.logicalDevice,
            .physicalDevice = context.physicalDevicePack.physicalDevice,
            .bufferSize = sizeof(NormalPassShaderUniform::Fragment)
        });
        normalPass.descriptorBindings[i][1].descriptorBuffers = std::vector<VkDescriptorBufferInfo>{
            VkDescriptorBufferInfo{
                .buffer = normalPass.fragmentShaderUniformBuffers[i].bufferPack.buffer,
                .offset = 0,
                .range = sizeof(NormalPassShaderUniform::Fragment)
            }
        };
    }
    
    normalPass.descriptorSets = DescriptorSet::Build(context.logicalDevice, normalPass.descriptorSetLayout, normalPass.descriptorPool, normalPass.descriptorBindings);
    
    normalPass.fsData.mipLevel = 0;
}

void NormalPass::Load(ContextInfo& context, NormalPassInfo& normalPass, ModelPack& modelPack)
{
    DescriptorPool::Reset(context.logicalDevice, normalPass.descriptorPool, 0);
    
    for(int j = 0; j < MAX_FRAMES_IN_FLIGHT; j++){
        normalPass.descriptorBindings[j][2].descriptorImages = {VkDescriptorImageInfo{
            .sampler = modelPack.models[0].material.normal.sampler,
            .imageView = VK_NULL_HANDLE,
        }};
        std::vector<VkDescriptorImageInfo> descriptorImages;
        for(int i = 0; i < modelPack.models.size(); i++){
            descriptorImages.push_back(VkDescriptorImageInfo{
                .sampler = VK_NULL_HANDLE,
                .imageView = modelPack.models[i].material.normal.imageView,
                .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
            });
        }
        normalPass.descriptorBindings[j][3].descriptorBuffers = modelPack.modelMatricesDescriptorBuffers[j];
        normalPass.descriptorBindings[j][4].descriptorImages = descriptorImages;
        normalPass.descriptorBindings[j][4].descriptorActualCount = descriptorImages.size();
    }
    
    normalPass.descriptorSets = DescriptorSet::Build(context.logicalDevice, normalPass.descriptorSetLayout, normalPass.descriptorPool, normalPass.descriptorBindings);
}

void NormalPass::Record(SceneInfo& scene, ContextInfo& context, uint32_t frame)
{
    VkCommandBuffer primaryCommandBuffer = scene.normalPass.primaryCommandBuffers[frame];
    
    Fence::Wait(context.logicalDevice, {scene.normalPass.fences[frame]}, VK_TRUE, UINT64_MAX);
    
    Fence::Reset(context.logicalDevice, {scene.normalPass.fences[frame]});
    
    CommandBuffer::Reset(primaryCommandBuffer);
    
    CommandBuffer::Begin(primaryCommandBuffer, 0, nullptr);
    
    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    barrier.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
    barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    barrier.image = scene.normalPass.images[frame].handle;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;
    vkCmdPipelineBarrier(primaryCommandBuffer, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);
    
    RenderPass::BeginRenderPass(RenderPassBeginConfiguration{
        .commandBuffer = primaryCommandBuffer,
        .frameBuffer = scene.normalPass.frameBuffers[frame],
        .resolution = context.swapChain.extent,
        .renderPass = scene.normalPass.renderPass,
        .clearValues = std::vector<VkClearValue>{
            { .color = {{0.0f, 0.0f, 0.0f, 1.0f}} }
        }
    });
    
    Viewport::Set(primaryCommandBuffer, 0, std::vector<VkViewport>{
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
        .commandBuffer = primaryCommandBuffer,
        .scissors = std::vector<VkRect2D>{
            VkRect2D{
                .offset = { 0, 0 },
                .extent = context.swapChain.extent
            }
        }
    });
    
    GraphicsPipeline::Bind(primaryCommandBuffer, scene.normalPass.graphicsPipeline.handle);
    
    Buffer::CopyMemory(CopyMemoryConfiguration{
        .data = scene.modelPack.modelSettings.data(),
        .dataSize = sizeof(scene.modelPack.modelSettings[0]) * scene.modelPack.modelSettings.size(),
        .memoryPointer = scene.modelPack.modelSettingsBuffers[frame].memoryPointer
    });
    
    Buffer::CopyMemory(CopyMemoryConfiguration{
        .data = &scene.normalPass.vsData,
        .dataSize = sizeof(NormalPassShaderUniform::Vertex),
        .memoryPointer = scene.normalPass.vertexShaderUniformBuffers[frame].memoryPointer
    });
    
    Buffer::CopyMemory(CopyMemoryConfiguration{
        .data = &scene.normalPass.fsData,
        .dataSize = sizeof(NormalPassShaderUniform::Fragment),
        .memoryPointer = scene.normalPass.fragmentShaderUniformBuffers[frame].memoryPointer
    });
        
    DescriptorSet::Bind(primaryCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, scene.normalPass.pipelineLayout, {scene.normalPass.descriptorSets[frame]});
    
    Buffer::BindVertexBuffer(BindVertexBufferConfiguration{
        .offsets = {0, 0, 0},
        .firstBinding = 0,
        .commandBuffer = primaryCommandBuffer,
        .vertexBuffers = {
            scene.modelPack.vertexBuffer.buffer,
            scene.modelPack.normalBuffer.buffer,
            scene.modelPack.textureCoordinatesBuffer.buffer
        }
    });
    
    Buffer::BindIndexBuffer(BindIndexBufferConfiguration{
        .commandBuffer = primaryCommandBuffer,
        .indexBuffer = scene.modelPack.indexBuffer.buffer,
        .type = VK_INDEX_TYPE_UINT32,
        .offset = 0
    });
      
    Drawer::DrawIndexedIndirect(primaryCommandBuffer, scene.occlusionCulling.outputDrawCommandsBuffers[frame].buffer, 0, scene.modelPack.drawCommands.size(), sizeof(VkDrawIndexedIndirectCommand));
    //Drawer::DrawIndexedIndirect(primaryCommandBuffer, scene.frustumCulling.visibleDrawCommandsBuffers[frame].buffer, 0, scene.modelPack.drawCommands.size(), sizeof(VkDrawIndexedIndirectCommand));
    
    RenderPass::EndRenderPass(primaryCommandBuffer) ;

    barrier = VkImageMemoryBarrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    barrier.image = scene.normalPass.images[frame].handle;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;
    vkCmdPipelineBarrier(primaryCommandBuffer, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

    CommandBuffer::End(primaryCommandBuffer) ;
}

void NormalPass::Submit(NormalPassInfo& normalPass, LightCullingInfo& lightCulling, ContextInfo& context, uint32_t frame)
{
    Queue::SubmitQueue(SubmitQueueConfiguration{
        .fence = normalPass.fences[frame],
        .graphicsQueue = context.queuePack.graphicsQueue,
        .submissionInfos = std::vector<SubmitQueueInfo>{
            SubmitQueueInfo{
                .waitSemaphores = {lightCulling.finishedSemaphores[frame]},
                .signalSemaphores = {normalPass.finishedSemaphores[frame]},
                .commandBuffers = {normalPass.primaryCommandBuffers[frame]},
                .waitStages = {VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT}
            }
        }
    });
}

void NormalPass::HandleWindowResize(ContextInfo& context, NormalPassInfo& normalPass, DepthPrepassInfo& depthPrepass, ModelPack& modelPack)
{
    size_t n = context.swapChain.imageCount;
    for(int i = 0; i < n; i++){
        Image::Destroy(context.logicalDevice, normalPass.images[i]);
        normalPass.images[i] = Image::BuildRawImage(BuildRawImageConfiguration{
            .logicalDevice = context.logicalDevice,
            .physicalDevice = context.physicalDevicePack.physicalDevice,
            .resolution = context.swapChain.extent,
            .depth = 1,
            .format = VK_FORMAT_R16G16B16A16_SFLOAT,
            .tiling = VK_IMAGE_TILING_OPTIMAL,
            .usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
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
        
    for(int i = 0; i < n; i++){
        ImageView::Destroy(context.logicalDevice, normalPass.imageViews[i]);
        normalPass.imageViews[i] = ImageView::Build({
            .logicalDevice = context.logicalDevice,
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .format = VK_FORMAT_R16G16B16A16_SFLOAT,
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .components = {
                .r = VK_COMPONENT_SWIZZLE_IDENTITY,
                .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                .a = VK_COMPONENT_SWIZZLE_IDENTITY
            },
            .levelCount = 1,
            .layerCount = 1,
            .baseMipLevel = 0,
            .baseArrayLayer = 0,
            .image = normalPass.images[i].handle
        });
        modelPack.descriptorBindings[i][3][0].descriptorImages = {VkDescriptorImageInfo{
            .sampler = VK_NULL_HANDLE,
            .imageView = normalPass.imageViews[i],
            .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        }};
    }
    
    for(int i = 0; i < n; i++){
        FrameBuffer::Destroy(context.logicalDevice, normalPass.frameBuffers[i]);
        normalPass.frameBuffers[i] = FrameBuffer::Create(context.logicalDevice, normalPass.renderPass, context.swapChain.extent, 1, {normalPass.imageViews[i], depthPrepass.imageViews[i]});
    }
    
    DescriptorPool::Reset(context.logicalDevice, normalPass.descriptorPool, 0);

    normalPass.descriptorSets = DescriptorSet::Build(context.logicalDevice, normalPass.descriptorSetLayout, normalPass.descriptorPool, normalPass.descriptorBindings);
}
