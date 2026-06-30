#include "Shadow.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include "Data/ShadowShaderUniform.h"




void Shadow::Build(ContextInfo& context, ModelPack& modelPack, LightInfo& light)
{
    ShadowInfo& shadow = light.shadow;
    
    shadow.config = light.config.shadowConfig;
    
    shadow.renderPass = RenderPass::BuildRenderPass(RenderPassBuildConfiguration{
        .logicalDevice = context.logicalDevice,
        .subpassDependencyBuildConfigurations = std::vector<SubpassDependencyBuildConfiguration>{
            SubpassDependencyBuildConfiguration{
                .srcSubpass = VK_SUBPASS_EXTERNAL,
                .dstSubpass = 0,
                .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
                .srcAccessMask = 0,
                .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
                .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT
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
                .finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL
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
        
    shadow.shader = Core::Shader::Build(context.logicalDevice, ShaderConfig{
        .filePaths = {"../Resources/Shaders/Shadow/Shadow_vs.spv", "../Resources/Shaders/Shadow/Shadow_fs.spv"},
        .stageFlags = {VK_SHADER_STAGE_VERTEX_BIT, VK_SHADER_STAGE_FRAGMENT_BIT}
    });
    
    shadow.vertexInput = VertexInput::Build("../Resources/shaders/Shadow/Shadow_vs.spv");
    
    shadow.descriptorBindingSchematic = std::vector<DescriptorBinding>{
        DescriptorBinding{
            .binding = 0,
            .descriptorActualCount = 1,
            .descriptorMaxCapacity = 1,
            .stageFlag = VK_SHADER_STAGE_VERTEX_BIT,
            .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            .descriptorImages = {VkDescriptorImageInfo{}},
            .descriptorBuffers = {VkDescriptorBufferInfo{}}
        },
        DescriptorBinding{
            .binding = 1,
            .descriptorActualCount = 1,
            .descriptorMaxCapacity = 1,
            .stageFlag = VK_SHADER_STAGE_FRAGMENT_BIT,
            .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            .descriptorImages = {VkDescriptorImageInfo{}},
            .descriptorBuffers = {VkDescriptorBufferInfo{}}
        },
        DescriptorBinding{
            .binding = 2,
            .descriptorActualCount = 1,
            .descriptorMaxCapacity = 1,
            .stageFlag = VK_SHADER_STAGE_VERTEX_BIT,
            .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
            .descriptorImages = {VkDescriptorImageInfo{}},
            .descriptorBuffers = {VkDescriptorBufferInfo{}}
        }
    };
        
    shadow.descriptorSetLayout = DescriptorSetLayout::Create(context.logicalDevice, shadow.descriptorBindingSchematic);
    
    shadow.pipelineLayout = PipelineLayout::Build(context.logicalDevice, {shadow.descriptorSetLayout});
    
    shadow.graphicsPipelineConfig = GraphicsPipelineConfig{
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
            .cullMode = VK_CULL_MODE_FRONT_BIT,
            .depthBiasEnable = VK_FALSE,
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
            .colorBlendAttachmentConfigs = {}
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
            .depthWriteEnable = VK_TRUE,
            .depthCompareOp = VK_COMPARE_OP_LESS,
            .stencilTestEnable = VK_FALSE,
            .depthBoundsTestEnable = VK_FALSE
        },
        .inputAssemblyStateConfig = {
            .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
            .primitiveRestartEnable = VK_FALSE
        },
    };

    shadow.graphicsPipeline = GraphicsPipeline::Build(context.logicalDevice, shadow.graphicsPipelineConfig, shadow.renderPass, shadow.pipelineLayout, shadow.shader, shadow.vertexInput);
    
    shadow.image = Image::BuildRawImage( BuildRawImageConfiguration{
        .logicalDevice = context.logicalDevice,
        .physicalDevice = context.physicalDevicePack.physicalDevice,
        .resolution = shadow.config.resolution,
        .depth = 1,
        .format = context.depthFormat,
        .tiling = VK_IMAGE_TILING_OPTIMAL,
        .usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT |
                 VK_IMAGE_USAGE_SAMPLED_BIT,
        .properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        .mipLevels = 1,
        .arrayLayers = 6,
        .imageFlag = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT,
        .imageType = VK_IMAGE_TYPE_2D,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE
    });
    
    shadow.imageView = ImageView::Build(ImageViewBuildInfo{
        .image = shadow.image.handle,
        .format = context.depthFormat,
        .viewType = VK_IMAGE_VIEW_TYPE_CUBE,
        .logicalDevice = context.logicalDevice,
        .aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT,
        .levelCount = 1,
        .layerCount = 6,
        .components = {
            .r = VK_COMPONENT_SWIZZLE_IDENTITY,
            .g = VK_COMPONENT_SWIZZLE_IDENTITY,
            .b = VK_COMPONENT_SWIZZLE_IDENTITY,
            .a = VK_COMPONENT_SWIZZLE_IDENTITY
        },
        .baseMipLevel = 0,
        .baseArrayLayer = 0,
    });
    
    shadow.bakingImageViews.resize(6);
    for(uint32_t i = 0; i < 6; i++)
        shadow.bakingImageViews[i] = ImageView::Build(ImageViewBuildInfo{
            .logicalDevice = context.logicalDevice,
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .format = context.depthFormat,
            .aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT,
            .components = {
                .r = VK_COMPONENT_SWIZZLE_IDENTITY,
                .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                .a = VK_COMPONENT_SWIZZLE_IDENTITY
            },
            .levelCount = 1,
            .layerCount = 1,
            .baseMipLevel = 0,
            .baseArrayLayer = i,
            .image = shadow.image.handle,
        });
    
    shadow.resolutions.resize(6);
    for(uint32_t i = 0; i < 6; i++)
        shadow.resolutions[i] = VkExtent2D{
            .width = std::max(1u, shadow.config.resolution.width >> 0),
            .height = std::max(1u, shadow.config.resolution.height >> 0)
        };
    
    shadow.frameBuffers.resize(6);
    for(uint32_t i = 0; i < 6; i++)
        shadow.frameBuffers[i] = FrameBuffer::Create(context.logicalDevice, shadow.renderPass, shadow.resolutions[i], 1, {shadow.bakingImageViews[i]});
    
    shadow.primaryCommandPool = CommandPool::Create(context.logicalDevice, context.physicalDevicePack.queueIndex, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
    
    shadow.primaryCommandBuffer = CommandBuffer::Allocate(context.logicalDevice, shadow.primaryCommandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1)[0];
    
    shadow.fence = Fence::Create(context.logicalDevice, VK_FENCE_CREATE_SIGNALED_BIT);
    
    shadow.semaphore = Semaphore::Build(context.logicalDevice);
    
    shadow.vertexShaderUniformBuffers.resize(MAX_NUM_LIGHTS);
    for(int k = 0; k < MAX_NUM_LIGHTS; k++){
        shadow.vertexShaderUniformBuffers[k].resize(6);
        for(uint32_t i = 0; i < 6; i++)
            shadow.vertexShaderUniformBuffers[k][i] = Buffer::BuildUniformBuffer(BuildUniformBufferConfiguration{
                .logicalDevice = context.logicalDevice,
                .physicalDevice = context.physicalDevicePack.physicalDevice,
                .bufferSize = sizeof(ShadowShaderUniform::Vertex)
            });
    }
        
    shadow.fragmentShaderUniformBuffers.resize(MAX_NUM_LIGHTS);
    for(int k = 0; k < MAX_NUM_LIGHTS; k++){
        shadow.fragmentShaderUniformBuffers[k].resize(6);
        for(uint32_t i = 0; i < 6; i++)
            shadow.fragmentShaderUniformBuffers[k][i] = Buffer::BuildUniformBuffer(BuildUniformBufferConfiguration{
                .logicalDevice = context.logicalDevice,
                .physicalDevice = context.physicalDevicePack.physicalDevice,
                .bufferSize = sizeof(ShadowShaderUniform::Fragment)
            });
    }
    
    shadow.descriptorBindings.resize(MAX_NUM_LIGHTS);
    for(int k = 0; k < MAX_NUM_LIGHTS; k++){
        shadow.descriptorBindings[k].assign(6, shadow.descriptorBindingSchematic);
        for(uint32_t i = 0; i < 6; i++){
            shadow.descriptorBindings[k][i][0].descriptorBuffers = {VkDescriptorBufferInfo{
                .buffer = shadow.vertexShaderUniformBuffers[k][i].bufferPack.buffer,
                .offset = 0,
                .range = sizeof(ShadowShaderUniform::Vertex)
            }};
            shadow.descriptorBindings[k][i][1].descriptorBuffers = {VkDescriptorBufferInfo{
                .buffer = shadow.fragmentShaderUniformBuffers[k][i].bufferPack.buffer,
                .offset = 0,
                .range = sizeof(ShadowShaderUniform::Fragment)
            }};
            shadow.descriptorBindings[k][i][2].descriptorBuffers = modelPack.modelMatricesDescriptorBuffers[0];
        }
    }
    
    shadow.descriptorPools.resize(MAX_NUM_LIGHTS);
    for(int k = 0; k < MAX_NUM_LIGHTS; k++)
        shadow.descriptorPools[k] = DescriptorPool::Build(context.logicalDevice, shadow.descriptorBindingSchematic, 6);
    
    shadow.descriptorSets.resize(MAX_NUM_LIGHTS);
    for(int k = 0; k < MAX_NUM_LIGHTS; k++){
        shadow.descriptorSets[k].resize(6);
        for(uint32_t i = 0; i < 6; i++)
            shadow.descriptorSets[k][i] = DescriptorSet::Build(context.logicalDevice, shadow.descriptorSetLayout, shadow.descriptorPools[k], {shadow.descriptorBindings[k][i]})[0];
    }
}

void Shadow::Load(ContextInfo& context, LightPack& lightPack, ModelPack& modelPack)
{
    for(size_t k = 0; k < modelPack.models.size(); k++){
        ModelInfo& model = modelPack.models[k];
        for(size_t m = 0; m < model.meshes.size(); m++){
            Mesh& mesh = model.meshes[m];
            
            for(size_t i = 0; i < mesh.vertices.size(); i++)
                lightPack.shadowVertices.push_back(mesh.vertices[i]);
            
            for(size_t i = 0; i < mesh.indices.size(); i++)
                lightPack.shadowIndices.push_back(mesh.indices[i]);
            
            lightPack.shadowDrawCommands.push_back(VkDrawIndexedIndirectCommand{
                .indexCount = static_cast<uint32_t>(mesh.indices.size()),
                .instanceCount = 1,
                .firstIndex = static_cast<uint32_t>(lightPack.shadowIndexOffset),
                .vertexOffset = static_cast<int32_t>(lightPack.shadowVertexOffset),
                .firstInstance = static_cast<uint32_t>(model.firstInstance),
            });
            
            lightPack.shadowVertexOffset += mesh.vertices.size();
            lightPack.shadowIndexOffset += mesh.indices.size();
        }
    }
    
    lightPack.shadowIndexBuffer = Buffer::BuildBufferWithData(BuildBufferWithDataConfiguration{
        .logicalDevice = context.logicalDevice,
        .physicalDevice = context.physicalDevicePack.physicalDevice,
        .graphicsQueue = context.queuePack.graphicsQueue,
        .commandPool = lightPack.primaryCommandPool,
        .data = lightPack.shadowIndices.data(),
        .bufferSize = sizeof(uint32_t) * lightPack.shadowIndices.size()
    });
    
    lightPack.shadowVertexBuffer = Buffer::BuildBufferWithData( BuildBufferWithDataConfiguration{
        .logicalDevice = context.logicalDevice,
        .physicalDevice = context.physicalDevicePack.physicalDevice,
        .graphicsQueue = context.queuePack.graphicsQueue,
        .commandPool = lightPack.primaryCommandPool,
        .data = lightPack.shadowVertices.data(),
        .bufferSize = sizeof(glm::vec3) * lightPack.shadowVertices.size()
    });
    
    lightPack.shadowDrawCommandsBuffer = Buffer::BuildBufferWithData( BuildBufferWithDataConfiguration{
        .logicalDevice = context.logicalDevice,
        .physicalDevice = context.physicalDevicePack.physicalDevice,
        .graphicsQueue = context.queuePack.graphicsQueue,
        .commandPool = lightPack.primaryCommandPool,
        .data = lightPack.shadowDrawCommands.data(),
        .bufferSize = sizeof(lightPack.shadowDrawCommands[0]) * lightPack.shadowDrawCommands.size()
    });
}

void Shadow::Bake(ContextInfo& context, ModelPack& modelPack, LightPack& lightPack, uint lightIndex)
{
    Record(context, modelPack, lightPack, lightIndex);
    Submit(context, lightPack.lights[lightIndex].shadow);
}

void Shadow::Record(ContextInfo& context, ModelPack& modelPack, LightPack& lightPack, uint lightIndex)
{
    LightInfo& light = lightPack.lights[lightIndex];
    
    ShadowInfo& shadow = light.shadow;
    
    CommandBuffer::Reset(shadow.primaryCommandBuffer);

    CommandBuffer::Begin(shadow.primaryCommandBuffer, 0, nullptr);
        
    for(uint32_t i = 0; i < 6; i++)
        RecordFace(context, modelPack, lightPack, light, i);

    CommandBuffer::End(shadow.primaryCommandBuffer);
}

void Shadow::RecordFace(ContextInfo& context, ModelPack& modelPack, LightPack& lightPack, LightInfo& light, uint32_t i)
{
    ShadowInfo& shadow = light.shadow;
    
    PipelineBarrier::InsertImageMemoryBarrier(InsertImageMemoryBarrierConfiguration{
        .graphicsQueue = context.queuePack.graphicsQueue,
        .logicalDevice = context.logicalDevice,
        .commandPool = shadow.primaryCommandPool,
        .dependencyFlags = 0,
        .commandBuffer = shadow.primaryCommandBuffer,
        .imageMemoryBarrierConfigurations = std::vector<ImageMemoryBarrierConfiguration>{
            ImageMemoryBarrierConfiguration{
                .image = shadow.image.handle,
                .levelCount = 1,
                .layerCount = 1,
                .baseMipLevel = 0,
                .baseArrayLayer = i,
                .oldLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL,
                .newLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                .aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT,
                .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED
            }
        }
    });

    
    RenderPass::BeginRenderPass( RenderPassBeginConfiguration{
        .commandBuffer = shadow.primaryCommandBuffer,
        .frameBuffer = shadow.frameBuffers[i],
        .resolution = shadow.resolutions[i],
        .renderPass = shadow.renderPass,
        .clearValues = std::vector<VkClearValue>{
            { .depthStencil = {1.0f, 0} }
        }
    });
    
    GraphicsPipeline::Bind(shadow.primaryCommandBuffer, shadow.graphicsPipeline.handle);
    
    Viewport::Set(shadow.primaryCommandBuffer, 0, std::vector<VkViewport>{
        VkViewport{
            .x = 0.0f,
            .y = 0.0f,
            .width = static_cast<float>(shadow.resolutions[i].width),
            .height = static_cast<float>(shadow.resolutions[i].height),
            .minDepth = 0.0f,
            .maxDepth = 1.0f
        }
    });
    
    Scissor::SetScissor( SetScissorConfiguration{
        .offset = 0,
        .commandBuffer = shadow.primaryCommandBuffer,
        .scissors = std::vector<VkRect2D>{
            VkRect2D{
                .offset = { 0, 0 },
                .extent = shadow.resolutions[i]
            }
        }
    });
    
    
    uint32_t k = light.index;
    
    DescriptorSet::Bind(shadow.primaryCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, shadow.pipelineLayout, {shadow.descriptorSets[k][i]});
    
    Buffer::CopyMemory(CopyMemoryConfiguration{
        .data = modelPack.modelMatrices.data(),
        .dataSize = sizeof(modelPack.modelMatrices[0]) * modelPack.modelMatrices.size(),
        .memoryPointer = modelPack.modelMatricesBuffers[0].memoryPointer
    });
    
    ShadowShaderUniform::Vertex vertexData{
        .viewMatrix = light.cubemapViewMatrices[i],
        .projectionMatrix = light.projectionMatrix
    };
    
    Buffer::CopyMemory(CopyMemoryConfiguration{
        .data = &vertexData,
        .dataSize = sizeof(ShadowShaderUniform::Vertex),
        .memoryPointer = shadow.vertexShaderUniformBuffers[k][i].memoryPointer
    });
    
    ShadowShaderUniform::Fragment fragmentData{
        .lightPosition = light.config.position,
        .lightFarPlane = glm::vec4(light.config.farClippingPlane)
    };
    
    Buffer::CopyMemory(CopyMemoryConfiguration{
        .data = &fragmentData,
        .dataSize = sizeof(ShadowShaderUniform::Fragment),
        .memoryPointer = shadow.fragmentShaderUniformBuffers[k][i].memoryPointer
    });
    
    Buffer::BindIndexBuffer(BindIndexBufferConfiguration{
        .commandBuffer = shadow.primaryCommandBuffer,
        .indexBuffer = lightPack.shadowIndexBuffer.buffer,
        .type = VK_INDEX_TYPE_UINT32,
        .offset = 0
    });

    Buffer::BindVertexBuffer(BindVertexBufferConfiguration{
        .offsets = {0},
        .firstBinding = 0,
        .commandBuffer = shadow.primaryCommandBuffer,
        .vertexBuffers = {lightPack.shadowVertexBuffer.buffer}
    }) ;
    
    Drawer::DrawIndexedIndirect(shadow.primaryCommandBuffer, lightPack.shadowDrawCommandsBuffer.buffer, 0, static_cast<uint32_t>(lightPack.shadowDrawCommands.size()), sizeof(VkDrawIndexedIndirectCommand));

    RenderPass::EndRenderPass(shadow.primaryCommandBuffer);
    
    
    PipelineBarrier::InsertImageMemoryBarrier(InsertImageMemoryBarrierConfiguration{
        .graphicsQueue = context.queuePack.graphicsQueue,
        .logicalDevice = context.logicalDevice,
        .commandPool = shadow.primaryCommandPool,
        .dependencyFlags = 0,
        .commandBuffer = shadow.primaryCommandBuffer,
        .imageMemoryBarrierConfigurations = std::vector<ImageMemoryBarrierConfiguration>{
            ImageMemoryBarrierConfiguration{
                .image = shadow.image.handle,
                .levelCount = 1,
                .layerCount = 1,
                .baseMipLevel = 0,
                .baseArrayLayer = i,
                .oldLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                .newLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL,
                .aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT,
                .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED
            }
        }
    });
}

void Shadow::Submit(ContextInfo& context, ShadowInfo& shadow)
{
    Queue::SubmitQueue(SubmitQueueConfiguration{
        .fence = shadow.fence,
        .graphicsQueue = context.queuePack.graphicsQueue,
        .submissionInfos = std::vector<SubmitQueueInfo>{
            SubmitQueueInfo{
                .waitSemaphores = {},
                .signalSemaphores = {shadow.semaphore},
                .commandBuffers = {shadow.primaryCommandBuffer},
                .waitStages = {}
            }
        }
    });
}
