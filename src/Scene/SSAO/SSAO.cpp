#include "SSAO.h"
#include "Data/SSAOConstants.h"
#include <random>





void SSAO::Build(ContextInfo& context, SSAOInfo& ssao, DepthPrepassInfo& depthPrepass, NormalPassInfo& normalPass, ModelPack& modelPack, VkSampler& nearestClampIntOpaqueBlackSampler)
{
    ssao.shader = Core::Shader::Build(context.logicalDevice, ShaderConfig{
        .filePaths = {
            vertexShaderFilePath,
            fragmentShaderFilePath
        },
        .stageFlags = {
            VK_SHADER_STAGE_VERTEX_BIT,
            VK_SHADER_STAGE_FRAGMENT_BIT
        }
    });
    
    ssao.vertexInput = VertexInput::Build(vertexShaderFilePath);
            
    ssao.renderPass = RenderPass::BuildRenderPass(RenderPassBuildConfiguration{
        .logicalDevice = context.logicalDevice,
        .subpassDependencyBuildConfigurations = std::vector<SubpassDependencyBuildConfiguration>{
            SubpassDependencyBuildConfiguration{
                .srcSubpass = VK_SUBPASS_EXTERNAL,
                .dstSubpass = 0,
                .srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                .srcAccessMask = VK_ACCESS_SHADER_READ_BIT,
                .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
                .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT
            }
        },
        .attachmentDescriptionBuildConfigurations = std::vector<AttachmentDescriptionBuildConfiguration>{
            AttachmentDescriptionBuildConfiguration{
                .format = VK_FORMAT_R8_UNORM,
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
        
    ssao.descriptorSetLayout = DescriptorSetLayout::Create(context.logicalDevice, descriptorBindingSchematic);

    ssao.pipelineLayout = PipelineLayout::Build(context.logicalDevice, {ssao.descriptorSetLayout});

    ssao.graphicsPipelineConfig = GraphicsPipelineConfig{
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
            .depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL,
            .stencilTestEnable = VK_FALSE,
            .depthBoundsTestEnable = VK_FALSE
        },
        .inputAssemblyStateConfig = {
            .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
            .primitiveRestartEnable = VK_FALSE
        },
    };

    ssao.graphicsPipeline = GraphicsPipeline::Build(context.logicalDevice, ssao.graphicsPipelineConfig, ssao.renderPass, ssao.pipelineLayout, ssao.shader, ssao.vertexInput);

    ssao.primaryCommandPool = CommandPool::Create(context.logicalDevice, context.physicalDevicePack.queueIndex, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
    
    ssao.primaryCommandBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        ssao.primaryCommandBuffers[i] = CommandBuffer::Allocate(context.logicalDevice, ssao.primaryCommandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1)[0];
        
    ssao.images.resize(MAX_FRAMES_IN_FLIGHT);
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        ssao.images[i] = Image::BuildRawImage(BuildRawImageConfiguration{
            .logicalDevice = context.logicalDevice,
            .physicalDevice = context.physicalDevicePack.physicalDevice,
            .resolution = context.swapChain.extent,
            .depth = 1,
            .format = VK_FORMAT_R8_UNORM,
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
    
    ssao.imageViews.resize(MAX_FRAMES_IN_FLIGHT);
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++){
        ssao.imageViews[i] = ImageView::Build({
            .logicalDevice = context.logicalDevice,
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .format = VK_FORMAT_R8_UNORM,
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
            .image = ssao.images[i].handle
        });
        modelPack.descriptorBindings[i][3][1].descriptorImages = {VkDescriptorImageInfo{
            .sampler = VK_NULL_HANDLE,
            .imageView = ssao.imageViews[i],
            .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        }};
    }
    
        
    ssao.frameBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        ssao.frameBuffers[i] = FrameBuffer::Create(context.logicalDevice, ssao.renderPass, context.swapChain.extent, 1, {ssao.imageViews[i]});
    
    ssao.fences.resize(MAX_FRAMES_IN_FLIGHT) ;
    for( int i = 0 ; i < MAX_FRAMES_IN_FLIGHT ; i++ )
        ssao.fences[i] = Fence::Create(context.logicalDevice, VK_FENCE_CREATE_SIGNALED_BIT);
    
    ssao.finishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    for( int i = 0 ; i < MAX_FRAMES_IN_FLIGHT ; i++ )
        ssao.finishedSemaphores[i] = Semaphore::Build(context.logicalDevice);
        
    for (int i = 0; i < NUM_SAMPLES; ++i) {
        glm::vec3 sample = glm::vec3(Random(-1.0, 1.0), Random(-1.0, 1.0), Random(0.0, 1.0));
        sample = glm::normalize(sample);
        sample *= Random(0.0, 1.0);
        float scale = float(i) / NUM_SAMPLES;
        scale = 0.1f + (1.0f - 0.1f) * (scale * scale);
        sample *= scale;
        ssao.samplingKernel[i] = glm::vec4(sample, 1.0f);
    }

    ssao.noiseTexture.config = TextureConfig{
        .resolution = static_cast<int>(noiseTextureResolution.width),
        .imageFlag = 0,
        .arrayLayers = 1,
        .mipMapped = false,
        .name = "Noise",
        .uiButtonID = "##Noise",
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .textureFormat = VK_FORMAT_R16G16B16A16_SFLOAT,
        .filePath = "",
        .addressMode = VK_SAMPLER_ADDRESS_MODE_REPEAT,
        .intermediateLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        .finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
    };
    ssao.noiseData = new uint16_t[noiseTextureImageSize];
    for(unsigned int i = 0; i < noiseTextureImageSize; i+=4){
        glm::vec3 noise = glm::normalize(glm::vec3(Random(-1.0f, 1.0f), Random(-1.0f, 1.0f), 0.0f));
        ssao.noiseData[i] = FloatToHalf(noise.x);
        ssao.noiseData[i+1] = FloatToHalf(noise.y);
        ssao.noiseData[i+2] = FloatToHalf(noise.z);
        ssao.noiseData[i+3] = FloatToHalf(1.0f);
    }

    ssao.noiseTexture.sourceImage = SourceImageInfo{
        .image = ssao.noiseData,
        .numChannels = 4,
        .numMipLevels = 1,
        .resolution = noiseTextureResolution,
        .imageSize = noiseTextureImageSize
    };
    Texture::BuildTextureWithImage(context, ssao.noiseTexture);

    
    ssao.fragmentShaderUniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        ssao.fragmentShaderUniformBuffers[i] = Buffer::BuildUniformBuffer(BuildUniformBufferConfiguration{
            .logicalDevice = context.logicalDevice,
            .physicalDevice = context.physicalDevicePack.physicalDevice,
            .bufferSize = sizeof(SSAOShaderUniform::Fragment)
        });
    
    ssao.descriptorPool = DescriptorPool::Build(context.logicalDevice, descriptorBindingSchematic, MAX_FRAMES_IN_FLIGHT);
        
    ssao.descriptorBindings.assign(MAX_FRAMES_IN_FLIGHT, descriptorBindingSchematic);
    for(int j = 0; j < MAX_FRAMES_IN_FLIGHT; j++){
        ssao.descriptorBindings[j][0].descriptorBuffers = std::vector<VkDescriptorBufferInfo>{
            VkDescriptorBufferInfo{
                .buffer = ssao.fragmentShaderUniformBuffers[j].bufferPack.buffer,
                .offset = 0,
                .range = sizeof(SSAOShaderUniform::Fragment)
            }
        };
        ssao.descriptorBindings[j][1].descriptorImages = {VkDescriptorImageInfo{
            .sampler = nearestClampIntOpaqueBlackSampler,
            .imageView = normalPass.imageViews[j],
            .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        }};
        ssao.descriptorBindings[j][2].descriptorImages = {VkDescriptorImageInfo{
            .sampler = depthPrepass.sampler,
            .imageView = depthPrepass.imageViews[j],
            .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        }};
        ssao.descriptorBindings[j][3].descriptorImages = {VkDescriptorImageInfo{
            .sampler = ssao.noiseTexture.sampler,
            .imageView = ssao.noiseTexture.imageView,
            .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        }};
    }
    
    ssao.descriptorSets = DescriptorSet::Build(context.logicalDevice, ssao.descriptorSetLayout, ssao.descriptorPool, ssao.descriptorBindings);
    
    ssao.fsData.samplingKernel = ssao.samplingKernel;
    ssao.fsData.noiseTextureResolution = glm::vec4(noiseTextureResolution.width, noiseTextureResolution.height, 0.0f, 0.0f);
    ssao.fsData.screenResolution_nearClippingPlane_farClippingPlane.r = context.swapChain.extent.width;
    ssao.fsData.screenResolution_nearClippingPlane_farClippingPlane.g = context.swapChain.extent.height;
}

uint16_t SSAO::FloatToHalf(float f)
{
    uint32_t x = *(uint32_t*)&f;
    
    uint16_t sign = (x >> 16) & 0x8000;
    uint32_t mant = x & 0x7fffff;
    int exp = ((x >> 23) & 0xff) - 112;
    
    if(exp <= 0){
        if(exp < -10)
            return sign;
        mant = (mant | 0x800000) >> (1 - exp);
        return sign | (mant >> 13);
    }

    if(exp >= 31)
        return sign | 0x7c00;
    
    return sign | (exp << 10) | (mant >> 13);
}

float SSAO::Random(float min, float max)
{
    static std::default_random_engine rng(std::random_device{}());
    std::uniform_real_distribution<float> dist(min, max);

    return dist(rng);
}

void SSAO::Record(ContextInfo& context, SSAOInfo& ssao, CameraPack& cameraPack, uint32_t frame)
{
    VkCommandBuffer primaryCommandBuffer = ssao.primaryCommandBuffers[frame];
    
    Fence::Wait(context.logicalDevice, {ssao.fences[frame]}, VK_TRUE, UINT64_MAX);
    
    Fence::Reset(context.logicalDevice, {ssao.fences[frame]});

    CommandBuffer::Reset(primaryCommandBuffer);
    
    CommandBuffer::Begin(primaryCommandBuffer, 0, nullptr);
    
    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    barrier.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
    barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    barrier.image = ssao.images[frame].handle;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;
    vkCmdPipelineBarrier(primaryCommandBuffer, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);
    
    RenderPass::BeginRenderPass(RenderPassBeginConfiguration{
        .commandBuffer = primaryCommandBuffer,
        .frameBuffer = ssao.frameBuffers[frame],
        .resolution = context.swapChain.extent,
        .renderPass = ssao.renderPass,
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
    
    GraphicsPipeline::Bind(primaryCommandBuffer, ssao.graphicsPipeline.handle);
    
    Buffer::CopyMemory(CopyMemoryConfiguration{
        .data = &ssao.fsData,
        .dataSize = sizeof(SSAOShaderUniform::Fragment),
        .memoryPointer = ssao.fragmentShaderUniformBuffers[frame].memoryPointer
    });
    
    DescriptorSet::Bind(primaryCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, ssao.pipelineLayout, {ssao.descriptorSets[frame]});
        
    Drawer::Draw(primaryCommandBuffer, 3, 1, 0, 0);
    
    RenderPass::EndRenderPass(primaryCommandBuffer) ;

    barrier = VkImageMemoryBarrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    barrier.image = ssao.images[frame].handle;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;
    vkCmdPipelineBarrier(primaryCommandBuffer, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);
    
    CommandBuffer::End(primaryCommandBuffer) ;
}

void SSAO::Submit(SSAOInfo& ssao, NormalPassInfo& normalPass, ContextInfo& context, uint32_t frame)
{
    Queue::SubmitQueue(SubmitQueueConfiguration{
        .fence = ssao.fences[frame],
        .graphicsQueue = context.queuePack.graphicsQueue,
        .submissionInfos = std::vector<SubmitQueueInfo>{
            SubmitQueueInfo{
                .waitSemaphores = {normalPass.finishedSemaphores[frame]},
                .signalSemaphores = {ssao.finishedSemaphores[frame]},
                .commandBuffers = {ssao.primaryCommandBuffers[frame]},
                .waitStages = {VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT}
            }
        }
    });
}

void SSAO::HandleWindowResize(ContextInfo& context, SSAOInfo& ssao, NormalPassInfo& normalPass, DepthPrepassInfo& depthPrepass, ModelPack& modelPack, VkSampler& nearestClampIntOpaqueBlackSampler)
{
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++){
        Image::Destroy(context.logicalDevice, ssao.images[i]);
        ssao.images[i] = Image::BuildRawImage(BuildRawImageConfiguration{
            .logicalDevice = context.logicalDevice,
            .physicalDevice = context.physicalDevicePack.physicalDevice,
            .resolution = context.swapChain.extent,
            .depth = 1,
            .format = VK_FORMAT_R8_UNORM,
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
    
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++){
        ImageView::Destroy(context.logicalDevice, ssao.imageViews[i]);
        ssao.imageViews[i] = ImageView::Build({
            .logicalDevice = context.logicalDevice,
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .format = VK_FORMAT_R8_UNORM,
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
            .image = ssao.images[i].handle
        });
        modelPack.descriptorBindings[i][3][1].descriptorImages = {VkDescriptorImageInfo{
            .sampler = VK_NULL_HANDLE,
            .imageView = ssao.imageViews[i],
            .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        }};
    }
    
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++){
        FrameBuffer::Destroy(context.logicalDevice, ssao.frameBuffers[i]);
        ssao.frameBuffers[i] = FrameBuffer::Create(context.logicalDevice, ssao.renderPass, context.swapChain.extent, 1, {ssao.imageViews[i]});
    }
    
    for(int j = 0; j < MAX_FRAMES_IN_FLIGHT; j++){
        ssao.descriptorBindings[j][1].descriptorImages = {VkDescriptorImageInfo{
            .sampler = nearestClampIntOpaqueBlackSampler,
            .imageView = normalPass.imageViews[j],
            .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        }};
        ssao.descriptorBindings[j][2].descriptorImages = {VkDescriptorImageInfo{
            .sampler = depthPrepass.sampler,
            .imageView = depthPrepass.imageViews[j],
            .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        }};
    }
    
    DescriptorPool::Reset(context.logicalDevice, ssao.descriptorPool, 0);
    
    ssao.descriptorSets = DescriptorSet::Build(context.logicalDevice, ssao.descriptorSetLayout, ssao.descriptorPool, ssao.descriptorBindings);
    
    ssao.fsData.screenResolution_nearClippingPlane_farClippingPlane.r = context.swapChain.extent.width;
    ssao.fsData.screenResolution_nearClippingPlane_farClippingPlane.g = context.swapChain.extent.height;
}
