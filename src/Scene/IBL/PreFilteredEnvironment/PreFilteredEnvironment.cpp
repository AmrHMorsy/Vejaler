#include "../../../../Resources/external/Include/stb_image/stb_image_write.h"
#include "PreFilteredEnvironment.h"
#include "../../Skybox/Variables/SkyboxVariables.h"





void PreFilteredEnvironment::Build(ContextInfo& context, PreFilteredEnvironmentInfo& preFilteredEnvironment, PreFilteredEnvironmentConfig config)
{
    preFilteredEnvironment.config = config;
    
    preFilteredEnvironment.numMipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(preFilteredEnvironment.config.resolution.width, preFilteredEnvironment.config.resolution.height)))) + 1;
    
    preFilteredEnvironment.primaryCommandPool = CommandPool::Create(context.logicalDevice, context.physicalDevicePack.queueIndex, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
    
    preFilteredEnvironment.primaryCommandBuffer = CommandBuffer::Allocate(context.logicalDevice, preFilteredEnvironment.primaryCommandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1)[0];    
    
    preFilteredEnvironment.renderPass = RenderPass::BuildRenderPass(RenderPassBuildConfiguration{
        .logicalDevice = context.logicalDevice,
        .subpassDependencyBuildConfigurations = std::vector<SubpassDependencyBuildConfiguration>{
            SubpassDependencyBuildConfiguration{
                .srcSubpass = VK_SUBPASS_EXTERNAL,
                .dstSubpass = 0,
                .srcAccessMask = 0,
                .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
                .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
                .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT
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
            }
        },
        .subpassDescriptionBuildConfigurations = std::vector<SubpassDescriptionBuildConfiguration>{
            SubpassDescriptionBuildConfiguration{
                .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
                .depthAttachmentReference = {},
                .colorAttachmentReferences = std::vector<VkAttachmentReference>{
                    VkAttachmentReference{
                        .attachment = 0,
                        .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
                    }
                }
            }
        }
    });
    
    
    preFilteredEnvironment.vertexBuffer = Buffer::BuildBufferWithData( BuildBufferWithDataConfiguration{
        .logicalDevice = context.logicalDevice,
        .physicalDevice = context.physicalDevicePack.physicalDevice,
        .graphicsQueue = context.queuePack.graphicsQueue,
        .commandPool = preFilteredEnvironment.primaryCommandPool,
        .data = skyboxVertices.data(),
        .bufferSize = sizeof(glm::vec3) * skyboxVertices.size()
    });    


    preFilteredEnvironment.shader = Core::Shader::Build(context.logicalDevice, ShaderConfig{
        .filePaths = {
            "../Resources/shaders/IBL/PreFiltrdEnv/PreFiltrdEnv_vs.spv",
            "../Resources/shaders/IBL/PreFiltrdEnv/PreFiltrdEnv_fs.spv"
        },
        .stageFlags = {
            VK_SHADER_STAGE_VERTEX_BIT,
            VK_SHADER_STAGE_FRAGMENT_BIT
        }
    });
    
    
    preFilteredEnvironment.vertexInput = VertexInput::Build("../Resources/shaders/IBL/PreFiltrdEnv/PreFiltrdEnv_vs.spv");
    
    
    preFilteredEnvironment.vertexShaderUniformBuffers.resize(6 * preFilteredEnvironment.numMipLevels);
    for(int mip = 0; mip < preFilteredEnvironment.numMipLevels; mip++){
        for(uint32_t i = 0; i < 6; i++)
            preFilteredEnvironment.vertexShaderUniformBuffers[(6*mip)+i] = Buffer::BuildUniformBuffer(BuildUniformBufferConfiguration{
                .logicalDevice = context.logicalDevice,
                .physicalDevice = context.physicalDevicePack.physicalDevice,
                .bufferSize = sizeof(PreFilteredEnvironmentShaderUniform::Vertex)
            });
    }
    
    
    preFilteredEnvironment.fragmentShaderUniformBuffers.resize(6 * preFilteredEnvironment.numMipLevels);
    for(int mip = 0; mip < preFilteredEnvironment.numMipLevels; mip++){
        for(uint32_t i = 0; i < 6; i++)
            preFilteredEnvironment.fragmentShaderUniformBuffers[(6*mip)+i] = Buffer::BuildUniformBuffer(BuildUniformBufferConfiguration{
                .logicalDevice = context.logicalDevice,
                .physicalDevice = context.physicalDevicePack.physicalDevice,
                .bufferSize = sizeof(PreFilteredEnvironmentShaderUniform::Fragment)
            });
    }
    
    
    preFilteredEnvironment.descriptorBindingSchematic =         std::vector<DescriptorBinding>{
        DescriptorBinding{
            .binding = 0,
            .descriptorActualCount = 1,
            .descriptorMaxCapacity = 1,
            .stageFlag = VK_SHADER_STAGE_VERTEX_BIT,
            .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            .descriptorImages = {VkDescriptorImageInfo{}},
            .descriptorBuffers = {VkDescriptorBufferInfo{}},
            .bufferSize = sizeof(PreFilteredEnvironmentShaderUniform::Vertex)
        },
        DescriptorBinding{
            .binding = 1,
            .descriptorActualCount = 1,
            .descriptorMaxCapacity = 1,
            .stageFlag = VK_SHADER_STAGE_FRAGMENT_BIT,
            .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            .descriptorImages = {VkDescriptorImageInfo{}},
            .descriptorBuffers = {VkDescriptorBufferInfo{}},
            .bufferSize = sizeof(PreFilteredEnvironmentShaderUniform::Fragment)
        },
        DescriptorBinding{
            .binding = 2,
            .descriptorActualCount = 1,
            .descriptorMaxCapacity = 1,
            .stageFlag = VK_SHADER_STAGE_FRAGMENT_BIT,
            .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            .descriptorImages = {VkDescriptorImageInfo{}},
            .descriptorBuffers = {VkDescriptorBufferInfo{}},
            .bufferSize = 0
        }
    };
    
    
    preFilteredEnvironment.descriptorPool = DescriptorPool::Build(context.logicalDevice, preFilteredEnvironment.descriptorBindingSchematic, 6 * preFilteredEnvironment.numMipLevels);
        
    preFilteredEnvironment.descriptorSetLayout = DescriptorSetLayout::Create(context.logicalDevice, preFilteredEnvironment.descriptorBindingSchematic);
    
    
    preFilteredEnvironment.descriptorBindings.assign(6 * preFilteredEnvironment.numMipLevels, preFilteredEnvironment.descriptorBindingSchematic);
    for(int mip = 0; mip < preFilteredEnvironment.numMipLevels; mip++){
        for(uint32_t i = 0; i < 6; i++){
            preFilteredEnvironment.descriptorBindings[(6*mip)+i][0].descriptorBuffers = {VkDescriptorBufferInfo{
                .buffer = preFilteredEnvironment.vertexShaderUniformBuffers[(6*mip)+i].bufferPack.buffer,
                .offset = 0,
                .range = sizeof(PreFilteredEnvironmentShaderUniform::Vertex)
            }};
            preFilteredEnvironment.descriptorBindings[(6*mip)+i][1].descriptorBuffers = {VkDescriptorBufferInfo{
                .buffer = preFilteredEnvironment.fragmentShaderUniformBuffers[(6*mip)+i].bufferPack.buffer,
                .offset = 0,
                .range = sizeof(PreFilteredEnvironmentShaderUniform::Fragment)
            }};
            preFilteredEnvironment.descriptorBindings[(6*mip)+i][2].descriptorImages = {VkDescriptorImageInfo{
                .sampler = preFilteredEnvironment.config.skyboxTexture.sampler,
                .imageView = preFilteredEnvironment.config.skyboxTexture.imageView,
                .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
            }} ;
        }
    }

    
    preFilteredEnvironment.descriptorSets.resize(6 * preFilteredEnvironment.numMipLevels);
    for(int mip = 0; mip < preFilteredEnvironment.numMipLevels; mip++){
        for(uint32_t i = 0; i < 6; i++)
            preFilteredEnvironment.descriptorSets[(6*mip)+i] = DescriptorSet::Build(context.logicalDevice, preFilteredEnvironment.descriptorSetLayout, preFilteredEnvironment.descriptorPool, {preFilteredEnvironment.descriptorBindings[(6*mip)+i]})[0];
    }
    

    preFilteredEnvironment.pipelineLayout = PipelineLayout::Build(context.logicalDevice, {preFilteredEnvironment.descriptorSetLayout});


    preFilteredEnvironment.graphicsPipelineConfig = GraphicsPipelineConfig{
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
            .cullMode = VK_CULL_MODE_NONE,
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
            .depthTestEnable = VK_FALSE,
            .depthWriteEnable = VK_FALSE,
            .depthCompareOp = VK_COMPARE_OP_LESS,
            .stencilTestEnable = VK_FALSE,
            .depthBoundsTestEnable = VK_FALSE
        },
        .inputAssemblyStateConfig = {
            .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
            .primitiveRestartEnable = VK_FALSE
        }
    };
    
    preFilteredEnvironment.graphicsPipeline = GraphicsPipeline::Build(context.logicalDevice, preFilteredEnvironment.graphicsPipelineConfig, preFilteredEnvironment.renderPass, preFilteredEnvironment.pipelineLayout, preFilteredEnvironment.shader, preFilteredEnvironment.vertexInput);
    
    preFilteredEnvironment.image = Image::BuildRawImage(BuildRawImageConfiguration{
        .logicalDevice = context.logicalDevice,
        .physicalDevice = context.physicalDevicePack.physicalDevice,
        .resolution = preFilteredEnvironment.config.resolution,
        .depth = 1,
        .format = VK_FORMAT_R16G16B16A16_SFLOAT,
        .tiling = VK_IMAGE_TILING_OPTIMAL,
        .usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        .properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        .mipLevels = preFilteredEnvironment.numMipLevels,
        .arrayLayers = 6,
        .imageFlag = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT,
        .imageType = VK_IMAGE_TYPE_2D,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE
    });
    
    
    PipelineBarrier::InsertImageMemoryBarrier(InsertImageMemoryBarrierConfiguration{
        .logicalDevice = context.logicalDevice,
        .commandPool = preFilteredEnvironment.primaryCommandPool,
        .graphicsQueue = context.queuePack.graphicsQueue,
        .dependencyFlags = 0,
        .imageMemoryBarrierConfigurations = std::vector<ImageMemoryBarrierConfiguration>{
            ImageMemoryBarrierConfiguration{
                .oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                .newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                .image = preFilteredEnvironment.image.handle,
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .baseMipLevel = 0,
                .levelCount = preFilteredEnvironment.numMipLevels,
                .baseArrayLayer = 0,
                .layerCount = 6
            }
        }
    });

    
    preFilteredEnvironment.bakingImageViews.resize(6 * preFilteredEnvironment.numMipLevels);
    for(int mip = 0; mip < preFilteredEnvironment.numMipLevels; mip++){
        for(uint32_t i = 0; i < 6; i++)
            preFilteredEnvironment.bakingImageViews[(6*mip)+i] = ImageView::Build({
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
                .baseMipLevel = static_cast<uint32_t>(mip),
                .baseArrayLayer = i,
                .image = preFilteredEnvironment.image.handle
            });
    }


    preFilteredEnvironment.imageView = ImageView::Build({
        .logicalDevice = context.logicalDevice,
        .viewType = VK_IMAGE_VIEW_TYPE_CUBE,
        .format = VK_FORMAT_R16G16B16A16_SFLOAT,
        .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
        .components = {
            .r = VK_COMPONENT_SWIZZLE_IDENTITY,
            .g = VK_COMPONENT_SWIZZLE_IDENTITY,
            .b = VK_COMPONENT_SWIZZLE_IDENTITY,
            .a = VK_COMPONENT_SWIZZLE_IDENTITY
        },
        .levelCount = preFilteredEnvironment.numMipLevels,
        .layerCount = 6,
        .baseMipLevel = 0,
        .baseArrayLayer = 0,
        .image = preFilteredEnvironment.image.handle
    });
    
    preFilteredEnvironment.samplerConfig = SamplerConfig{
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
        .maxLod = float(preFilteredEnvironment.numMipLevels)
    };
    preFilteredEnvironment.sampler = Sampler::Build(context.logicalDevice, context.physicalDevicePack.physicalDevice, preFilteredEnvironment.samplerConfig);
    
    
    preFilteredEnvironment.resolutions.resize(preFilteredEnvironment.numMipLevels);
    for(uint32_t mip = 0; mip < preFilteredEnvironment.numMipLevels; mip++)
        preFilteredEnvironment.resolutions[mip] = VkExtent2D{
            .width = std::max(1u, preFilteredEnvironment.config.resolution.width >> mip),
            .height = std::max(1u, preFilteredEnvironment.config.resolution.height >> mip)
        };
    
    
    preFilteredEnvironment.frameBuffers.resize(6 * preFilteredEnvironment.numMipLevels);
    for(int mip = 0; mip < preFilteredEnvironment.numMipLevels; mip++){
        for(uint32_t i = 0; i < 6; i++)
            preFilteredEnvironment.frameBuffers[(6*mip)+i] = FrameBuffer::Create(context.logicalDevice, preFilteredEnvironment.renderPass, preFilteredEnvironment.config.resolution, 1, {preFilteredEnvironment.bakingImageViews[(6*mip)+i]});
    }
    
    
    glm::vec4 roughnessAndresolution;
    float roughness;
    uint32_t resolutionWidth = preFilteredEnvironment.config.resolution.width;
    uint32_t resolutionheight = preFilteredEnvironment.config.resolution.height;
    preFilteredEnvironment.fragmentShaderDatas.resize(6 * preFilteredEnvironment.numMipLevels);
    for(int mip = 0; mip < preFilteredEnvironment.numMipLevels; mip++){
        roughness = (float)mip / (float)(preFilteredEnvironment.numMipLevels - 1);
        roughnessAndresolution = glm::vec4(roughness, resolutionWidth, resolutionheight, 0.0f);
        for(uint32_t i = 0; i < 6; i++)
            preFilteredEnvironment.fragmentShaderDatas[(6*mip)+i] = PreFilteredEnvironmentShaderUniform::Fragment{
                .roughnessAndresolution = roughnessAndresolution
            };
    }
    
    
    glm::mat4 skyboxProjectionMatrix = SkyboxProjectionMatrix();
    preFilteredEnvironment.vertexShaderDatas.resize(6 * preFilteredEnvironment.numMipLevels);
    for(int mip = 0; mip < preFilteredEnvironment.numMipLevels; mip++){
        for(uint32_t i = 0; i < 6; i++)
            preFilteredEnvironment.vertexShaderDatas[(6*mip)+i] = PreFilteredEnvironmentShaderUniform::Vertex{
                .projection = skyboxProjectionMatrix,
                .view = skyboxViewMatrices[i]
            };
    }

    
    preFilteredEnvironment.fence = Fence::Create(context.logicalDevice, 0);

    
    preFilteredEnvironment.secondaryCommandPools.resize(6 * preFilteredEnvironment.numMipLevels);
    for(int mip = 0; mip < preFilteredEnvironment.numMipLevels; mip++){
        for(uint32_t i = 0; i < 6; i++)
            preFilteredEnvironment.secondaryCommandPools[(6*mip)+i] = CommandPool::Create(context.logicalDevice, context.physicalDevicePack.queueIndex, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
    }
    
    
    preFilteredEnvironment.secondaryCommandBuffers.resize(6 * preFilteredEnvironment.numMipLevels);
    for(int mip = 0; mip < preFilteredEnvironment.numMipLevels; mip++){
        for(uint32_t i = 0; i < 6; i++)
            preFilteredEnvironment.secondaryCommandBuffers[(6*mip)+i] = CommandBuffer::Allocate(context.logicalDevice, preFilteredEnvironment.secondaryCommandPools[i], VK_COMMAND_BUFFER_LEVEL_SECONDARY, 1)[0];
    }
    
    
    preFilteredEnvironment.threads.resize(6 * preFilteredEnvironment.numMipLevels);    
}


void PreFilteredEnvironment::Record(ContextInfo& context, PreFilteredEnvironmentInfo& preFilteredEnvironment)
{
    for(uint32_t mip = 0; mip < preFilteredEnvironment.numMipLevels; mip++){
        for( uint32_t i = 0; i < 6; i++ )
            preFilteredEnvironment.threads[(6*mip)+i] = std::thread([&, i, mip](){
                RecordFace(context, preFilteredEnvironment, mip, i);
            });
    }
    
    for(uint32_t mip = 0; mip < preFilteredEnvironment.numMipLevels; mip++){
        for( uint32_t i = 0; i < 6; i++ )
            preFilteredEnvironment.threads[(6*mip)+i].join();
    }

    
    
    CommandBuffer::Begin(preFilteredEnvironment.primaryCommandBuffer, 0, nullptr);
    
    for(uint32_t mip = 0; mip < preFilteredEnvironment.numMipLevels; mip++){
        for( uint32_t i = 0; i < 6; i++ ){

            RenderPass::BeginRenderPass(RenderPassBeginConfiguration{
                .resolution = preFilteredEnvironment.resolutions[mip],
                .renderPass = preFilteredEnvironment.renderPass,
                .frameBuffer = preFilteredEnvironment.frameBuffers[(6*mip)+i],
                .commandBuffer = preFilteredEnvironment.primaryCommandBuffer,
                .clearValues = {VkClearValue{
                    .color = {{0.0f, 0.0f, 0.0f, 1.0f}}
                }}
            });
            
            CommandBuffer::Execute(preFilteredEnvironment.primaryCommandBuffer, {preFilteredEnvironment.secondaryCommandBuffers[(6*mip)+i]});
            
            RenderPass::EndRenderPass(preFilteredEnvironment.primaryCommandBuffer);
        }
    }
    

    CommandBuffer::End(preFilteredEnvironment.primaryCommandBuffer) ;
}


void PreFilteredEnvironment::RecordFace(ContextInfo& context, PreFilteredEnvironmentInfo& preFilteredEnvironment, uint32_t mip, uint32_t i)
{
    VkCommandBuffer commandBuffer = preFilteredEnvironment.secondaryCommandBuffers[(6*mip)+i];
    
    
    VkCommandBufferInheritanceInfo commandBufferInheritanceInfo{
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO,
        .renderPass = preFilteredEnvironment.renderPass,
        .framebuffer = preFilteredEnvironment.frameBuffers[(6*mip)+i],
        .subpass = 0
    };
    
    
    CommandBuffer::Begin(commandBuffer, VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT, &commandBufferInheritanceInfo);
    
    Viewport::Set(commandBuffer, 0, std::vector<VkViewport>{
        VkViewport{
            .x = 0.0f,
            .y = 0.0f,
            .width = static_cast<float>(preFilteredEnvironment.resolutions[mip].width),
            .height = static_cast<float>(preFilteredEnvironment.resolutions[mip].height),
            .minDepth = 0.0f,
            .maxDepth = 1.0f
        }
    });
    
    
    Scissor::SetScissor( SetScissorConfiguration{
        .offset = 0,
        .commandBuffer = commandBuffer,
        .scissors = std::vector<VkRect2D>{
            VkRect2D{
                .offset = {0, 0},
                .extent = VkExtent2D{
                    .width = preFilteredEnvironment.resolutions[mip].width,
                    .height = preFilteredEnvironment.resolutions[mip].height
                }
            }
        }
    });
    
    
    Buffer::CopyMemory(CopyMemoryConfiguration{
        .data = &preFilteredEnvironment.vertexShaderDatas[(6*mip)+i],
        .dataSize = sizeof(PreFilteredEnvironmentShaderUniform::Vertex),
        .memoryPointer = preFilteredEnvironment.vertexShaderUniformBuffers[(6*mip)+i].memoryPointer
    });
    
    
    Buffer::CopyMemory(CopyMemoryConfiguration{
        .data = &preFilteredEnvironment.fragmentShaderDatas[(6*mip)+i],
        .dataSize = sizeof(PreFilteredEnvironmentShaderUniform::Fragment),
        .memoryPointer = preFilteredEnvironment.fragmentShaderUniformBuffers[(6*mip)+i].memoryPointer
    });
    
    
    GraphicsPipeline::Bind(commandBuffer, preFilteredEnvironment.graphicsPipeline.handle) ;

           
    DescriptorSet::Bind(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, preFilteredEnvironment.pipelineLayout, {preFilteredEnvironment.descriptorSets[(6*mip)+i]});
        
    Buffer::BindVertexBuffer(BindVertexBufferConfiguration{
        .offsets = {0},
        .firstBinding = 0,
        .commandBuffer = commandBuffer,
        .vertexBuffers = {preFilteredEnvironment.vertexBuffer.buffer}
    });

    
    Drawer::Draw(commandBuffer, static_cast<uint32_t>(skyboxVertices.size()), 1, 0, 0);
    
    CommandBuffer::End(commandBuffer) ;
}


void PreFilteredEnvironment::Submit(ContextInfo& context, PreFilteredEnvironmentInfo& preFilteredEnvironment, VkSemaphore skyboxSemaphore)
{
    Queue::SubmitQueue(SubmitQueueConfiguration{
        .fence = preFilteredEnvironment.fence,
        .graphicsQueue = context.queuePack.graphicsQueue,
        .submissionInfos = std::vector<SubmitQueueInfo>{
            SubmitQueueInfo{
                .waitSemaphores = {skyboxSemaphore},
                .signalSemaphores = {},
                .commandBuffers = {preFilteredEnvironment.primaryCommandBuffer},
                .waitStages = {VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT}
            }
        }
    });
}
