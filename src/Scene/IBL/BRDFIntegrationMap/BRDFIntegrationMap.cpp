#include "BRDFIntegrationMap.h"
#include "Variables/BRDFIntegrationVertices.h"
#include "Variables/BRDFIntegrationTextureCoordinates.h"



void BRDFIntegrationMap::Build(ContextInfo& context, BRDFIntegrationMapInfo& brdfIntegrationMap, VkExtent2D resolution)
{
    brdfIntegrationMap.resolution = resolution;
    
    
    brdfIntegrationMap.primaryCommandPool = CommandPool::Create(context.logicalDevice, context.physicalDevicePack.queueIndex, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
    
    
    brdfIntegrationMap.renderPass = RenderPass::BuildRenderPass(RenderPassBuildConfiguration{
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
    
    
    brdfIntegrationMap.vertexBuffer = Buffer::BuildBufferWithData(BuildBufferWithDataConfiguration{
        .logicalDevice = context.logicalDevice,
        .physicalDevice = context.physicalDevicePack.physicalDevice,
        .graphicsQueue = context.queuePack.graphicsQueue,
        .commandPool = brdfIntegrationMap.primaryCommandPool,
        .data = brdfIntegrationVertices.data(),
        .bufferSize = sizeof(glm::vec3) * brdfIntegrationVertices.size()
    });
    
    brdfIntegrationMap.textureCoordinateBuffer = Buffer::BuildBufferWithData(BuildBufferWithDataConfiguration{
        .logicalDevice = context.logicalDevice,
        .physicalDevice = context.physicalDevicePack.physicalDevice,
        .graphicsQueue = context.queuePack.graphicsQueue,
        .commandPool = brdfIntegrationMap.primaryCommandPool,
        .data = brdfIntegrationTextureCoordinates.data(),
        .bufferSize = sizeof(glm::vec2) * brdfIntegrationTextureCoordinates.size()
    });
    
    
    brdfIntegrationMap.shader = Core::Shader::Build(context.logicalDevice, ShaderConfig{
        .filePaths = {
            "../Resources/shaders/IBL/BRDF_IntegrationMap/BRDF_IntegrationMap_vs.spv",
            "../Resources/shaders/IBL/BRDF_IntegrationMap/BRDF_IntegrationMap_fs.spv"},
        .stageFlags = {
            VK_SHADER_STAGE_VERTEX_BIT,
            VK_SHADER_STAGE_FRAGMENT_BIT
        }
    });
    
    
    brdfIntegrationMap.vertexInput = VertexInput::Build("../Resources/shaders/IBL/BRDF_IntegrationMap/BRDF_IntegrationMap_vs.spv");
    
    
    brdfIntegrationMap.pipelineLayout = PipelineLayout::Build(context.logicalDevice, {});
    
    
    brdfIntegrationMap.graphicsPipelineConfig = GraphicsPipelineConfig{
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
    
    
    brdfIntegrationMap.graphicsPipeline = GraphicsPipeline::Build(context.logicalDevice, brdfIntegrationMap.graphicsPipelineConfig, brdfIntegrationMap.renderPass, brdfIntegrationMap.pipelineLayout, brdfIntegrationMap.shader, brdfIntegrationMap.vertexInput);
    
    
    brdfIntegrationMap.image = Image::BuildRawImage(BuildRawImageConfiguration{
        .logicalDevice = context.logicalDevice,
        .physicalDevice = context.physicalDevicePack.physicalDevice,
        .resolution = brdfIntegrationMap.resolution,
        .depth = 1,
        .format = VK_FORMAT_R16G16B16A16_SFLOAT,
        .tiling = VK_IMAGE_TILING_OPTIMAL,
        .usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        .properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        .mipLevels = 1,
        .arrayLayers = 1,
        .imageFlag = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT,
        .imageType = VK_IMAGE_TYPE_2D,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE
    });
    
    
    PipelineBarrier::InsertImageMemoryBarrier(InsertImageMemoryBarrierConfiguration{
        .logicalDevice = context.logicalDevice,
        .commandPool = brdfIntegrationMap.primaryCommandPool,
        .graphicsQueue = context.queuePack.graphicsQueue,
        .dependencyFlags = 0,
        .imageMemoryBarrierConfigurations = std::vector<ImageMemoryBarrierConfiguration>{
            ImageMemoryBarrierConfiguration{
                .oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                .newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                .image = brdfIntegrationMap.image.handle,
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .baseMipLevel = 0,
                .levelCount = 1,
                .baseArrayLayer = 0,
                .layerCount = 1
            }
        }
    });
    
    
    brdfIntegrationMap.imageView = ImageView::Build({
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
        .image = brdfIntegrationMap.image.handle
    });


    brdfIntegrationMap.primaryCommandBuffer = CommandBuffer::Allocate(context.logicalDevice, brdfIntegrationMap.primaryCommandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1)[0];
    
    
    brdfIntegrationMap.frameBuffer = FrameBuffer::Create(context.logicalDevice, brdfIntegrationMap.renderPass, brdfIntegrationMap.resolution, 1, {brdfIntegrationMap.imageView});    
    
    brdfIntegrationMap.fence = Fence::Create(context.logicalDevice, 0);
}

void BRDFIntegrationMap::Record(ContextInfo& context, BRDFIntegrationMapInfo& brdfIntegrationMap)
{
    CommandBuffer::Begin(brdfIntegrationMap.primaryCommandBuffer, 0, nullptr);
    
    RenderPass::BeginRenderPass( RenderPassBeginConfiguration{
        .resolution = brdfIntegrationMap.resolution,
        .renderPass = brdfIntegrationMap.renderPass,
        .frameBuffer = brdfIntegrationMap.frameBuffer,
        .commandBuffer = brdfIntegrationMap.primaryCommandBuffer,
        .clearValues = {VkClearValue{
            .color = {{0.0f, 0.0f, 0.0f, 1.0f}}
        }}
    }) ;
    
    Viewport::Set(brdfIntegrationMap.primaryCommandBuffer, 0, std::vector<VkViewport>{
        VkViewport{
            .x = 0.0f,
            .y = 0.0f,
            .width = static_cast<float>(brdfIntegrationMap.resolution.width),
            .height = static_cast<float>(brdfIntegrationMap.resolution.height),
            .minDepth = 0.0f,
            .maxDepth = 1.0f
        }
    });
    
    
    Scissor::SetScissor( SetScissorConfiguration{
        .offset = 0,
        .commandBuffer = brdfIntegrationMap.primaryCommandBuffer,
        .scissors = std::vector<VkRect2D>{
            VkRect2D{
                .offset = { 0, 0 },
                .extent = brdfIntegrationMap.resolution
            }
        }
    }) ;


    GraphicsPipeline::Bind(brdfIntegrationMap.primaryCommandBuffer, brdfIntegrationMap.graphicsPipeline.handle);
    
    
    Buffer::BindVertexBuffer(BindVertexBufferConfiguration{
        .offsets = {0, 0},
        .firstBinding = 0,
        .commandBuffer = brdfIntegrationMap.primaryCommandBuffer,
        .vertexBuffers = {
            brdfIntegrationMap.vertexBuffer.buffer,
            brdfIntegrationMap.textureCoordinateBuffer.buffer
        }
    });
    
    
    Drawer::Draw(brdfIntegrationMap.primaryCommandBuffer, static_cast<uint32_t>(brdfIntegrationVertices.size()), 1, 0, 0);
    
    RenderPass::EndRenderPass(brdfIntegrationMap.primaryCommandBuffer) ;

    
    CommandBuffer::End(brdfIntegrationMap.primaryCommandBuffer);
}

void BRDFIntegrationMap::Submit(ContextInfo& context, BRDFIntegrationMapInfo& brdfIntegrationMap)
{
    Queue::SubmitQueue(SubmitQueueConfiguration{
        .fence = brdfIntegrationMap.fence,
        .graphicsQueue = context.queuePack.graphicsQueue,
        .submissionInfos = std::vector<SubmitQueueInfo>{
            SubmitQueueInfo{
                .waitSemaphores = {},
                .signalSemaphores = {},
                .commandBuffers = {brdfIntegrationMap.primaryCommandBuffer},
                .waitStages = {}
            }
        }
    });
}
