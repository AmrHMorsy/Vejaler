#include "Irradiance.h"
#include "Data/IrradianceShaderUniform.h"
#include "../../Skybox/Variables/SkyboxVariables.h"




void Irradiance::Build(ContextInfo& context, IrradianceInfo& irradiance, IrradianceConfig config)
{
    irradiance.config = config;
    
    irradiance.descriptorBindingSchematic =         std::vector<DescriptorBinding>{
        DescriptorBinding{
            .binding = 0,
            .descriptorActualCount = 1,
            .descriptorMaxCapacity = 1,
            .stageFlag = VK_SHADER_STAGE_VERTEX_BIT,
            .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            .descriptorImages = {VkDescriptorImageInfo{}},
            .descriptorBuffers = {VkDescriptorBufferInfo{}},
            .bufferSize = sizeof(IrradianceShaderUniform::Vertex)
        },
        DescriptorBinding{
            .binding = 1,
            .descriptorActualCount = 1,
            .descriptorMaxCapacity = 1,
            .stageFlag = VK_SHADER_STAGE_FRAGMENT_BIT,
            .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            .descriptorImages = {VkDescriptorImageInfo{}},
            .descriptorBuffers = {VkDescriptorBufferInfo{}},
            .bufferSize = 0
        }
    };
    
    
    irradiance.primaryCommandPool = CommandPool::Create(context.logicalDevice, context.physicalDevicePack.queueIndex, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
    
    
    irradiance.primaryCommandBuffer = CommandBuffer::Allocate(context.logicalDevice, irradiance.primaryCommandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1)[0];
    
    irradiance.renderPass = RenderPass::BuildRenderPass(RenderPassBuildConfiguration{
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
    
    
    irradiance.vertexBuffer = Buffer::BuildBufferWithData( BuildBufferWithDataConfiguration{
        .logicalDevice = context.logicalDevice,
        .physicalDevice = context.physicalDevicePack.physicalDevice,
        .graphicsQueue = context.queuePack.graphicsQueue,
        .commandPool = irradiance.primaryCommandPool,
        .data = skyboxVertices.data(),
        .bufferSize = sizeof(glm::vec3) * skyboxVertices.size()
    });
    
    
    irradiance.shader = Core::Shader::Build(context.logicalDevice, ShaderConfig{
        .filePaths = {
            "../Resources/shaders/IBL/Irradiance/Irradiance_vs.spv",
            "../Resources/shaders/IBL/Irradiance/Irradiance_fs.spv"
        },
            .stageFlags = {
                VK_SHADER_STAGE_VERTEX_BIT,
                VK_SHADER_STAGE_FRAGMENT_BIT
            }
    });
    
    
    irradiance.vertexInput = VertexInput::Build("../Resources/shaders/IBL/Irradiance/Irradiance_vs.spv");
    
    
    irradiance.uniformBuffers.resize(6);
    for(int i = 0; i < 6; i++)
        irradiance.uniformBuffers[i] = Buffer::BuildUniformBuffer(BuildUniformBufferConfiguration{
            .logicalDevice = context.logicalDevice,
            .physicalDevice = context.physicalDevicePack.physicalDevice,
            .bufferSize = sizeof(IrradianceShaderUniform::Vertex)
        });
    
    
    irradiance.descriptorPool = DescriptorPool::Build(context.logicalDevice, irradiance.descriptorBindingSchematic, 6);    
    
    irradiance.descriptorSetLayout = DescriptorSetLayout::Create(context.logicalDevice, irradiance.descriptorBindingSchematic);
    
    
    irradiance.pipelineLayout = PipelineLayout::Build(context.logicalDevice, {irradiance.descriptorSetLayout});
    
    
    irradiance.descriptorBindings.resize(6);
    for(int i = 0; i < 6; i++){
        irradiance.descriptorBindings[i] = irradiance.descriptorBindingSchematic;
        irradiance.descriptorBindings[i][0].descriptorBuffers = {VkDescriptorBufferInfo{
            .buffer = irradiance.uniformBuffers[i].bufferPack.buffer,
            .offset = 0,
            .range = sizeof(IrradianceShaderUniform::Vertex)
        }};
        irradiance.descriptorBindings[i][1].descriptorImages = {VkDescriptorImageInfo{
            .sampler = irradiance.config.skyboxTexturePack.sampler,
            .imageView = irradiance.config.skyboxTexturePack.imageView,
            .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        }};
    }
    
    irradiance.descriptorSets.resize(6);
    for(int i = 0; i < 6; i++)
        irradiance.descriptorSets[i] = DescriptorSet::Build( context.logicalDevice, irradiance.descriptorSetLayout, irradiance.descriptorPool, {irradiance.descriptorBindings[i]})[0];        

    irradiance.graphicsPipelineConfig = GraphicsPipelineConfig{
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
        },
    };
    
    irradiance.graphicsPipeline = GraphicsPipeline::Build(context.logicalDevice, irradiance.graphicsPipelineConfig, irradiance.renderPass, irradiance.pipelineLayout, irradiance.shader, irradiance.vertexInput);


    irradiance.texture = Texture::BuildRawTexture(RawTextureBuildConfiguration{
        .resolution = irradiance.config.resolution,
        .logicalDevice = context.logicalDevice,
        .physicalDevice = context.physicalDevicePack.physicalDevice,
        .commandPool = irradiance.primaryCommandPool,
        .graphicsQueue = context.queuePack.graphicsQueue,
        .addressMode = VK_SAMPLER_ADDRESS_MODE_REPEAT,
        .textureFormat = VK_FORMAT_R16G16B16A16_SFLOAT,
        .viewType = VK_IMAGE_VIEW_TYPE_CUBE,
        .imageFlag = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT,
        .arrayLayers = 6,
        .intermediateLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        .finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        .numMipLevels = 1
    });
    
    
    irradiance.imageViews.resize(6) ;
    for(uint32_t i = 0; i < 6; i++)
        irradiance.imageViews[i] = ImageView::Build(ImageViewBuildInfo{
            .logicalDevice = context.logicalDevice,
            .format = VK_FORMAT_R16G16B16A16_SFLOAT,
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .components = {
                .r = VK_COMPONENT_SWIZZLE_IDENTITY,
                .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                .a = VK_COMPONENT_SWIZZLE_IDENTITY
            },
            .image = irradiance.texture.image.handle,
            .levelCount = 1,
            .layerCount = 1,
            .baseMipLevel = 0,
            .baseArrayLayer = i
        });
    
    
    irradiance.frameBuffers.resize(6);
    for(uint32_t i = 0; i < 6; i++)
        irradiance.frameBuffers[i] = FrameBuffer::Create(context.logicalDevice, irradiance.renderPass, irradiance.config.resolution, 1, {irradiance.imageViews[i]});
    
    irradiance.fence = Fence::Create(context.logicalDevice, 0);
    
    
    irradiance.secondaryCommandPools.resize(6);
    for(int i = 0; i < 6; i++)
        irradiance.secondaryCommandPools[i] = CommandPool::Create(context.logicalDevice, context.physicalDevicePack.queueIndex, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
    
    
    irradiance.secondaryCommandBuffers.resize(6);
    for(int i = 0; i < 6; i++){
        irradiance.secondaryCommandBuffers[i] = CommandBuffer::Allocate(context.logicalDevice, irradiance.secondaryCommandPools[i], VK_COMMAND_BUFFER_LEVEL_SECONDARY, 1)[0];
    }

    
    glm::mat4 projectionMatrix = SkyboxProjectionMatrix();
    irradiance.vertexShaderUniformDatas.resize(6);
    for(int i = 0; i < 6; i++){
        irradiance.vertexShaderUniformDatas[i] = {};
        irradiance.vertexShaderUniformDatas[i].projection = projectionMatrix;
        irradiance.vertexShaderUniformDatas[i].view = skyboxViewMatrices[i] ;
    }
    
    
    irradiance.threads.resize(6);
}

void Irradiance::Record(ContextInfo& context, IrradianceInfo& irradiance)
{
    for(uint32_t i = 0; i < 6; i++)
        irradiance.threads[i] = std::thread([&, i](){
            RecordFace(context, irradiance, i);
        });
    
    for(uint32_t i = 0; i < 6; i++)
        irradiance.threads[i].join();
    
    
    CommandBuffer::Begin(irradiance.primaryCommandBuffer, 0, nullptr);
    
    for( uint32_t i = 0; i < 6; i++ ){
        
        RenderPass::BeginRenderPass( RenderPassBeginConfiguration{
            .resolution = irradiance.config.resolution,
            .renderPass = irradiance.renderPass,
            .frameBuffer = irradiance.frameBuffers[i],
            .commandBuffer = irradiance.primaryCommandBuffer,
            .clearValues = {VkClearValue{
                .color = {{0.0f, 0.0f, 0.0f, 1.0f}}
            }}
        });
        
        
        CommandBuffer::Execute(irradiance.primaryCommandBuffer, {irradiance.secondaryCommandBuffers[i]});
        
        RenderPass::EndRenderPass(irradiance.primaryCommandBuffer);
    }
    
    
    CommandBuffer::End(irradiance.primaryCommandBuffer);
}

void Irradiance::RecordFace(ContextInfo& context, IrradianceInfo& irradiance, int faceIndex)
{
    VkCommandBufferInheritanceInfo commandBufferInheritanceInfo{
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO,
        .renderPass = irradiance.renderPass,
        .framebuffer = irradiance.frameBuffers[faceIndex],
        .subpass = 0
    };
    
    
    CommandBuffer::Begin(irradiance.secondaryCommandBuffers[faceIndex], VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT, &commandBufferInheritanceInfo);
    
    Viewport::Set(irradiance.secondaryCommandBuffers[faceIndex], 0, std::vector<VkViewport>{
        VkViewport{
            .x = 0.0f,
            .y = 0.0f,
            .width = static_cast<float>(irradiance.config.resolution.width),
            .height = static_cast<float>(irradiance.config.resolution.height),
            .minDepth = 0.0f,
            .maxDepth = 1.0f
        }
    });
    
    
    Scissor::SetScissor( SetScissorConfiguration{
        .offset = 0,
        .commandBuffer = irradiance.secondaryCommandBuffers[faceIndex],
        .scissors = std::vector<VkRect2D>{
            VkRect2D{
                .offset = { 0, 0 },
                .extent = irradiance.config.resolution
            }
        }
    }) ;

    
    Buffer::CopyMemory(CopyMemoryConfiguration{
        .data = &irradiance.vertexShaderUniformDatas[faceIndex],
        .dataSize = sizeof(IrradianceShaderUniform::Vertex),
        .memoryPointer = irradiance.uniformBuffers[faceIndex].memoryPointer
    });
    
    
    GraphicsPipeline::Bind(irradiance.secondaryCommandBuffers[faceIndex], irradiance.graphicsPipeline.handle) ;

    
    DescriptorSet::Bind(irradiance.secondaryCommandBuffers[faceIndex], VK_PIPELINE_BIND_POINT_GRAPHICS, irradiance.pipelineLayout, {irradiance.descriptorSets[faceIndex]});
    
    Buffer::BindVertexBuffer(BindVertexBufferConfiguration{
        .offsets = {0},
        .firstBinding = 0,
        .commandBuffer = irradiance.secondaryCommandBuffers[faceIndex],
        .vertexBuffers = {irradiance.vertexBuffer.buffer}
    });
    
    
    Drawer::Draw(irradiance.secondaryCommandBuffers[faceIndex], static_cast<uint32_t>(skyboxVertices.size()), 1, 0, 0);
    
    CommandBuffer::End(irradiance.secondaryCommandBuffers[faceIndex]) ;
}

void Irradiance::Submit(ContextInfo& context, IrradianceInfo& irradiance, VkSemaphore skyboxSemaphore)
{
    Queue::SubmitQueue(SubmitQueueConfiguration{
        .fence = irradiance.fence,
        .graphicsQueue = context.queuePack.graphicsQueue,
        .submissionInfos = std::vector<SubmitQueueInfo>{
            SubmitQueueInfo{
                .waitSemaphores = {skyboxSemaphore},
                .signalSemaphores = {},
                .commandBuffers = {irradiance.primaryCommandBuffer},
                .waitStages = {VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT}
            }
        }
    });
}
