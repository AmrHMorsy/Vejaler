#define STB_IMAGE_IMPLEMENTATION
#include "../../../Resources/external/include/stb_image/stb_image.h"
#include "Skybox.h"
#include "Variables/SkyboxVariables.h"



void Skybox::Build(ContextInfo& context, SceneInfo& scene)
{
    SkyboxPack& skyboxPack = scene.skyboxPack;

    skyboxPack.shaders.resize(NUM_SKYBOX_SHADERS);
    skyboxPack.shaders[HDRTOCUBEMAP] = Core::Shader::Build(context.logicalDevice, ShaderConfig{
        .filePaths = {
            hdrToCubemapVertexShaderFilePath,
            hdrToCubemapFragmentShaderFilePath
        },
        .stageFlags = {
            VK_SHADER_STAGE_VERTEX_BIT,
            VK_SHADER_STAGE_FRAGMENT_BIT
        }
    });
    skyboxPack.shaders[SKYBOX] = Core::Shader::Build(context.logicalDevice, ShaderConfig{
        .filePaths = {
            skyboxVertexShaderFilePath,
            skyboxFragmentShaderFilePath
        },
        .stageFlags = {
            VK_SHADER_STAGE_VERTEX_BIT,
            VK_SHADER_STAGE_FRAGMENT_BIT
        }
    });
    
    skyboxPack.vertexInputs.resize(NUM_SKYBOX_SHADERS);
    skyboxPack.vertexInputs[HDRTOCUBEMAP] = VertexInput::Build(hdrToCubemapVertexShaderFilePath);
    skyboxPack.vertexInputs[SKYBOX] = VertexInput::Build(skyboxVertexShaderFilePath);
    
    
    skyboxPack.descriptorBindingSchematics.resize(NUM_SKYBOX_SHADERS);
    skyboxPack.descriptorBindingSchematics[HDRTOCUBEMAP] = std::vector<DescriptorBinding>{
        DescriptorBinding{
            .binding = 0,
            .descriptorActualCount = 1,
            .descriptorMaxCapacity = 1,
            .stageFlag = VK_SHADER_STAGE_VERTEX_BIT,
            .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            .descriptorImages = {VkDescriptorImageInfo{}},
            .descriptorBuffers = {VkDescriptorBufferInfo{}},
            .bufferSize = sizeof(HDRToCubemapVertexShaderUniformVariables)
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
    skyboxPack.descriptorBindingSchematics[SKYBOX] = std::vector<DescriptorBinding>{
        DescriptorBinding{
            .binding = 0,
            .descriptorActualCount = 1,
            .descriptorMaxCapacity = 1,
            .stageFlag = VK_SHADER_STAGE_VERTEX_BIT,
            .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            .descriptorImages = {VkDescriptorImageInfo{}},
            .descriptorBuffers = {VkDescriptorBufferInfo{}},
            .bufferSize = sizeof(SkyboxVertexShaderUniformVariables)
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

    
    skyboxPack.descriptorSetLayouts.resize(NUM_SKYBOX_SHADERS);
    skyboxPack.descriptorSetLayouts[HDRTOCUBEMAP] = DescriptorSetLayout::Create(context.logicalDevice, skyboxPack.descriptorBindingSchematics[HDRTOCUBEMAP]);
    skyboxPack.descriptorSetLayouts[SKYBOX] = DescriptorSetLayout::Create(context.logicalDevice, skyboxPack.descriptorBindingSchematics[SKYBOX]);
    
    
    skyboxPack.pipelineLayouts.resize(NUM_SKYBOX_SHADERS);
    skyboxPack.pipelineLayouts[HDRTOCUBEMAP] = PipelineLayout::Build(context.logicalDevice, {skyboxPack.descriptorSetLayouts[HDRTOCUBEMAP]});
    skyboxPack.pipelineLayouts[SKYBOX] = PipelineLayout::Build(context.logicalDevice, {skyboxPack.descriptorSetLayouts[SKYBOX]});
    
    
    skyboxPack.hdrToCubemapRenderPass = RenderPass::BuildRenderPass(RenderPassBuildConfiguration{
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

    skyboxPack.graphicsPipelines.resize(NUM_SKYBOX_SHADERS);
    skyboxPack.graphicsPipelineConfigs.resize(NUM_SKYBOX_SHADERS);
    
    
    skyboxPack.graphicsPipelineConfigs[HDRTOCUBEMAP] = GraphicsPipelineConfig{
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
            .depthCompareOp=VK_COMPARE_OP_LESS,
            .stencilTestEnable = VK_FALSE,
            .depthBoundsTestEnable = VK_FALSE
        },
        .inputAssemblyStateConfig = {
            .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
            .primitiveRestartEnable = VK_FALSE
        }
    };

    skyboxPack.graphicsPipelines[HDRTOCUBEMAP] = GraphicsPipeline::Build(context.logicalDevice, skyboxPack.graphicsPipelineConfigs[HDRTOCUBEMAP], skyboxPack.hdrToCubemapRenderPass, skyboxPack.pipelineLayouts[HDRTOCUBEMAP], skyboxPack.shaders[HDRTOCUBEMAP], skyboxPack.vertexInputs[HDRTOCUBEMAP]);
    

    skyboxPack.graphicsPipelineConfigs[SKYBOX] = GraphicsPipelineConfig{
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

    skyboxPack.graphicsPipelines[SKYBOX] = GraphicsPipeline::Build(context.logicalDevice, skyboxPack.graphicsPipelineConfigs[SKYBOX], scene.renderPass, skyboxPack.pipelineLayouts[SKYBOX], skyboxPack.shaders[SKYBOX], skyboxPack.vertexInputs[SKYBOX]);


    skyboxPack.mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(bakingResolution.width, bakingResolution.height)))) + 1;

    
    skyboxPack.secondaryCommandPools.resize(skyboxPack.mipLevels*6);
    for(uint32_t mip = 0; mip < skyboxPack.mipLevels; mip++){
        for(uint32_t i = 0; i < 6; i++)
            skyboxPack.secondaryCommandPools[(6*mip)+i] = CommandPool::Create(context.logicalDevice, context.physicalDevicePack.queueIndex, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
    }
    
    
    skyboxPack.secondaryCommandBuffers.resize(skyboxPack.mipLevels * 6);
    for(uint32_t mip = 0; mip < skyboxPack.mipLevels; mip++){
        for(uint32_t i = 0; i < 6; i++)
            skyboxPack.secondaryCommandBuffers[(6*mip)+i] = CommandBuffer::Allocate(context.logicalDevice, skyboxPack.secondaryCommandPools[i], VK_COMMAND_BUFFER_LEVEL_SECONDARY, 1)[0];
    }
    
    
    skyboxPack.threads.resize(skyboxPack.mipLevels * 6);
    
    
    skyboxPack.hdrToCubemapPrimaryCommandPool = CommandPool::Create(context.logicalDevice, context.physicalDevicePack.queueIndex, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);

    
    skyboxPack.hdrToCubemapPrimaryCommandBuffer = CommandBuffer::Allocate(context.logicalDevice, skyboxPack.hdrToCubemapPrimaryCommandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1)[0];
}

nlohmann::json Skybox::Save(SkyboxPack& skyboxPack)
{
    nlohmann::json skyboxesJSON = nlohmann::json::array();
    for(size_t k = 0; k < skyboxPack.skyboxes.size(); k++){
        nlohmann::json skyboxJSON = {
            {"filePath", skyboxPack.skyboxes[k].filePath},
        };
        skyboxesJSON.push_back(skyboxJSON);
    }
    
    return skyboxesJSON;
}

void Skybox::Load(ContextInfo& context, SceneInfo& scene, const nlohmann::json& modelsJSON)
{
    for(size_t i = 0; i < modelsJSON.size(); i++)
        Add(context, scene, modelsJSON[i]["filePath"]);
}

void Skybox::Add(ContextInfo& context, SceneInfo& scene, const std::string& filePath)
{
    SkyboxPack& skyboxPack = scene.skyboxPack;
    skyboxPack.bakingInProgress = true;
    skyboxPack.skyboxes.resize(skyboxPack.skyboxes.size() + 1);
    skyboxPack.names.resize(skyboxPack.names.size() + 1);
    skyboxPack.selected = skyboxPack.skyboxes.size() - 1;
    SkyboxInfo& skybox = skyboxPack.skyboxes[skyboxPack.selected];
    
    skybox.filePath = filePath;
    
    for(size_t i = 0; i < skyboxPack.skyboxes.size(); i++){
        skyboxPack.skyboxes[i].name = "Skybox " + std::to_string(i);
        skyboxPack.names[i] = skyboxPack.skyboxes[i].name.c_str();
    }
    
    /*
    if(skyboxPack.sceneSecondaryCommandBufferIndex == -1){
        size_t n = scene.secondaryCommandBuffers[0].size();
        skyboxPack.sceneSecondaryCommandBufferIndex = n;
        for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++){
            scene.secondaryCommandPools[i].resize(n + 1);
            scene.secondaryCommandPools[i][n] = CommandPool::Create(context.logicalDevice, context.physicalDevicePack.queueIndex, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
        }
        for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++){
            scene.secondaryCommandBuffers[i].resize(n + 1);
            scene.secondaryCommandBuffers[i][n] = CommandBuffer::Allocate(context.logicalDevice, scene.secondaryCommandPools[i][n], VK_COMMAND_BUFFER_LEVEL_SECONDARY, 1)[0];
        }
    }
    */
    
    skybox.skyboxTexturePack = Texture::BuildRawTexture(RawTextureBuildConfiguration{
        .resolution = bakingResolution,
        .logicalDevice = context.logicalDevice,
        .physicalDevice = context.physicalDevicePack.physicalDevice,
        .commandPool = skyboxPack.hdrToCubemapPrimaryCommandPool,
        .graphicsQueue = context.queuePack.graphicsQueue,
        .addressMode = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
        .textureFormat = VK_FORMAT_R16G16B16A16_SFLOAT,
        .viewType = VK_IMAGE_VIEW_TYPE_CUBE,
        .imageFlag = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT,
        .arrayLayers = 6,
        .intermediateLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        .finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        .numMipLevels = skyboxPack.mipLevels
    });
    
    
    std::thread skyboxThread = std::thread([&](){
        skybox.hdrToCubemapUniformBuffers.resize(6 * skyboxPack.mipLevels);
        for(uint32_t mip = 0; mip < skyboxPack.mipLevels; mip++){
            for( uint32_t i = 0; i < 6; i++ )
                skybox.hdrToCubemapUniformBuffers[(6*mip)+i] = Buffer::BuildUniformBuffer(BuildUniformBufferConfiguration{
                    .logicalDevice = context.logicalDevice,
                    .physicalDevice = context.physicalDevicePack.physicalDevice,
                    .bufferSize = sizeof(HDRToCubemapVertexShaderUniformVariables)
                });
        }
        
        
        skybox.skyboxUniformBufferInfo.resize(MAX_FRAMES_IN_FLIGHT);
        for( int i = 0 ; i < MAX_FRAMES_IN_FLIGHT; i++ )
            skybox.skyboxUniformBufferInfo[i] = Buffer::BuildUniformBuffer(BuildUniformBufferConfiguration{
                .logicalDevice = context.logicalDevice,
                .physicalDevice = context.physicalDevicePack.physicalDevice,
                .bufferSize = sizeof(SkyboxVertexShaderUniformVariables)
            });
        
        
        skybox.hdrToCubemapTextureInfo.config = TextureConfig{
            .filePath = filePath.c_str(),
            .addressMode = VK_SAMPLER_ADDRESS_MODE_REPEAT,
            .textureFormat = VK_FORMAT_R16G16B16A16_SFLOAT,
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .imageFlag = 0,
            .arrayLayers = 1,
            .intermediateLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            .finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
            .mipMapped = false
        };
        skybox.hdrToCubemapTextureInfo.sourceImage = SourceImage::LoadHDR(skybox.hdrToCubemapTextureInfo.config.filePath.c_str());
        Texture::BuildTextureWithImage(context, skybox.hdrToCubemapTextureInfo);
        
        
        skybox.hdrToCubemapDescriptorBindings.resize(6 * skyboxPack.mipLevels);
        for(uint32_t mip = 0; mip < skyboxPack.mipLevels; mip++){
            for( uint32_t i = 0; i < 6; i++ ){
                skybox.hdrToCubemapDescriptorBindings[(6*mip)+i].resize(2);
                skybox.hdrToCubemapDescriptorBindings[(6*mip)+i][0] = DescriptorBinding{
                    .binding = 0,
                    .descriptorActualCount = 1,
                    .descriptorMaxCapacity = 1,
                    .stageFlag = VK_SHADER_STAGE_VERTEX_BIT,
                    .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                    .descriptorImages = {VkDescriptorImageInfo{}},
                    .descriptorBuffers = {VkDescriptorBufferInfo{
                        .buffer = skybox.hdrToCubemapUniformBuffers[(6*mip)+i].bufferPack.buffer,
                        .offset = 0,
                        .range = {sizeof(HDRToCubemapVertexShaderUniformVariables)}}}
                } ;
                skybox.hdrToCubemapDescriptorBindings[(6*mip)+i][1] = DescriptorBinding{
                    .binding = 1,
                    .descriptorActualCount = 1,
                    .descriptorMaxCapacity = 1,
                    .stageFlag = VK_SHADER_STAGE_FRAGMENT_BIT,
                    .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                    .descriptorImages = {VkDescriptorImageInfo{
                        .sampler = skybox.hdrToCubemapTextureInfo.sampler,
                        .imageView = skybox.hdrToCubemapTextureInfo.imageView,
                        .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL}},
                        .descriptorBuffers = {VkDescriptorBufferInfo{}}
                } ;
            }
        }
        
        
        skybox.hdrToCubemapDescriptorPool = DescriptorPool::Build(context.logicalDevice,  skybox.hdrToCubemapDescriptorBindings[0], 6 * skyboxPack.mipLevels);
        
        skybox.hdrToCubemapDescriptorSets.resize(6 * skyboxPack.mipLevels);
        for(uint32_t mip = 0; mip < skyboxPack.mipLevels; mip++){
            for( uint32_t i = 0; i < 6; i++ )
                skybox.hdrToCubemapDescriptorSets[(6*mip)+i] = DescriptorSet::Build(context.logicalDevice, skyboxPack.descriptorSetLayouts[HDRTOCUBEMAP], skybox.hdrToCubemapDescriptorPool, {skybox.hdrToCubemapDescriptorBindings[(6*mip)+i]})[0];
        }
        
        
        skybox.skyboxImageViews.resize(skyboxPack.mipLevels * 6);
        for(uint32_t mip = 0; mip < skyboxPack.mipLevels; mip++){
            for(uint32_t i = 0; i < 6; i++)
                skybox.skyboxImageViews[(6*mip)+i] = ImageView::Build(ImageViewBuildInfo{
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
                    .image = skybox.skyboxTexturePack.image.handle,
                    .levelCount = 1,
                    .layerCount = 1,
                    .baseMipLevel = mip,
                    .baseArrayLayer = i
                }) ;
        }
        
        
        skybox.skyboxDescriptorBindings.resize(MAX_FRAMES_IN_FLIGHT) ;
        for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++){
            skybox.skyboxDescriptorBindings[i].resize(2) ;
            skybox.skyboxDescriptorBindings[i][0] = DescriptorBinding{
                .binding = 0,
                .descriptorActualCount = 1,
                .descriptorMaxCapacity = 1,
                .stageFlag = VK_SHADER_STAGE_VERTEX_BIT,
                .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                .descriptorImages = {VkDescriptorImageInfo{}},
                .descriptorBuffers = {VkDescriptorBufferInfo{
                    .buffer = skybox.skyboxUniformBufferInfo[i].bufferPack.buffer,
                    .offset = 0,
                    .range = {sizeof(SkyboxVertexShaderUniformVariables)}}} } ;
            skybox.skyboxDescriptorBindings[i][1] = DescriptorBinding{
                .binding = 1,
                .descriptorActualCount = 1,
                .descriptorMaxCapacity = 1,
                .stageFlag = VK_SHADER_STAGE_FRAGMENT_BIT,
                .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                .descriptorImages = {VkDescriptorImageInfo{
                    .sampler = skybox.skyboxTexturePack.sampler,
                    .imageView = skybox.skyboxTexturePack.imageView,
                    .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL}},
                    .descriptorBuffers = {VkDescriptorBufferInfo{}}
            };
        }
        
        
        skybox.skyboxDescriptorPool = DescriptorPool::Build(context.logicalDevice, skybox.skyboxDescriptorBindings[0], MAX_FRAMES_IN_FLIGHT);

        
        skybox.skyboxDescriptorSets = DescriptorSet::Build(context.logicalDevice, skyboxPack.descriptorSetLayouts[SKYBOX], skybox.skyboxDescriptorPool, skybox.skyboxDescriptorBindings);
        
        skybox.skyboxVertexBufferInfo = Buffer::BuildBufferWithData( BuildBufferWithDataConfiguration{
            .logicalDevice = context.logicalDevice,
            .physicalDevice = context.physicalDevicePack.physicalDevice,
            .graphicsQueue = context.queuePack.graphicsQueue,
            .commandPool = skyboxPack.hdrToCubemapPrimaryCommandPool,
            .data = skyboxVertices.data(),
            .bufferSize = sizeof(glm::vec3) * skyboxVertices.size()
        });
        
        
        skybox.hdrToCubemapResolutions.resize(skyboxPack.mipLevels);
        skybox.resolutionStrings.resize(skyboxPack.mipLevels);
        skybox.resolutionStringsPtr.resize(skyboxPack.mipLevels);
        for(uint32_t mip = 0; mip < skyboxPack.mipLevels; mip++){
            VkExtent2D resolution = VkExtent2D{
                .width = std::max(1u, bakingResolution.width >> mip),
                .height = std::max(1u, bakingResolution.height >> mip)
            };
            skybox.hdrToCubemapResolutions[mip] = resolution;
            skybox.resolutionStrings[mip] = std::to_string(resolution.width) + " x " + std::to_string(resolution.height);
            skybox.resolutionStringsPtr[mip] = skybox.resolutionStrings[mip].c_str();
        }
        
        
        skybox.hdrToCubemapFrameBuffers.resize(skyboxPack.mipLevels);
        for(uint32_t mip = 0; mip < skyboxPack.mipLevels; mip++){
            skybox.hdrToCubemapFrameBuffers[mip].resize(6);
            for(uint32_t i = 0; i < 6; i++)
                skybox.hdrToCubemapFrameBuffers[mip][i] = FrameBuffer::Create(context.logicalDevice, skyboxPack.hdrToCubemapRenderPass, skybox.hdrToCubemapResolutions[mip], 1, {skybox.skyboxImageViews[(6*mip)+i]});
        }
        
        
        skybox.hdrToCubemapFence = Fence::Create(context.logicalDevice, 0);
        
        
        glm::mat4 skyboxProjectionMatrix = SkyboxProjectionMatrix();
        skybox.hdrToCubemapVertexShaderUniformDatas.resize(6 * skyboxPack.mipLevels);
        for(uint32_t mip = 0; mip < skyboxPack.mipLevels; mip++){
            for(uint32_t i = 0; i < 6; i++)
                skybox.hdrToCubemapVertexShaderUniformDatas[(6*mip)+i] = HDRToCubemapVertexShaderUniformVariables{
                    .view = skyboxViewMatrices[i],
                    .projection = skyboxProjectionMatrix
                };
        }
        
        
        skybox.hdrToCubemapIrradianceSemaphore = Semaphore::Build(context.logicalDevice);
        
        
        skybox.hdrToCubemapPreFilteredEnvironmentSemaphore = Semaphore::Build(context.logicalDevice);
        
        
        Record(context, skyboxPack);
    });
        
    
    std::thread iblThread = std::thread([&](){
        IBL::Build(context, skybox);
        IBL::Record(context, skybox);
    });

    
    iblThread.join();
    skyboxThread.join();
    
    Submit(context, skyboxPack);
    IBL::Submit(context, skybox);
    
    Fence::Wait(context.logicalDevice, {skybox.hdrToCubemapFence, skybox.ibl.irradiance.fence, skybox.ibl.preFilteredEnvironment.fence, skybox.ibl.brdfIntegrationMap.fence}, VK_TRUE, UINT64_MAX);
    
    skybox.ready = true;
    skyboxPack.bakingInProgress = false;
    
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++){
        scene.modelPack.descriptorBindings[i][4][0].descriptorImages = {VkDescriptorImageInfo{
            .sampler = VK_NULL_HANDLE,
            .imageView = skybox.ibl.irradiance.texture.imageView,
            .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        }};
        scene.modelPack.descriptorBindings[i][4][1].descriptorImages = {VkDescriptorImageInfo{
            .sampler = VK_NULL_HANDLE,
            .imageView = skybox.ibl.brdfIntegrationMap.imageView,
            .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        }};
        scene.modelPack.descriptorBindings[i][4][2].descriptorImages = {VkDescriptorImageInfo{
            .sampler = VK_NULL_HANDLE,
            .imageView = skybox.ibl.preFilteredEnvironment.imageView,
            .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        }};
    }
    
    Model::UpdateDescriptorSets(context, scene.modelPack);
    
    skybox.vsData.mipLevel_isHDRTonemappingEnabled_isGammaCorrectEnabled = glm::vec4(skybox.selectedResolution, skybox.isHDRTonemappingEnabled, skybox.isGammaCorrectEnabled, 0.0f);
}

void Skybox::Record(ContextInfo& context, SkyboxPack& skyboxPack)
{
    SkyboxInfo& skybox = skyboxPack.skyboxes[skyboxPack.selected];
    
    for(uint32_t mip = 0; mip < skyboxPack.mipLevels; mip++){
        for( uint32_t i = 0; i < 6; i++ )
            skyboxPack.threads[(6*mip)+i] = std::thread([&, i, mip](){
                RecordFace(context, skyboxPack, mip, i);
            });
    }
    
    for(uint32_t mip = 0; mip < skyboxPack.mipLevels; mip++){
        for( uint32_t i = 0; i < 6; i++ )
            skyboxPack.threads[(6*mip)+i].join();
    }
    
    CommandBuffer::Begin(skyboxPack.hdrToCubemapPrimaryCommandBuffer, 0, nullptr);
        
    for(uint32_t mip = 0; mip < skyboxPack.mipLevels; mip++){
        for( uint32_t i = 0; i < 6; i++ ){
            RenderPass::BeginRenderPass( RenderPassBeginConfiguration{
                .resolution = skybox.hdrToCubemapResolutions[mip],
                .renderPass = skyboxPack.hdrToCubemapRenderPass,
                .frameBuffer = skybox.hdrToCubemapFrameBuffers[mip][i],
                .commandBuffer = skyboxPack.hdrToCubemapPrimaryCommandBuffer,
                .clearValues = {VkClearValue{
                    .color = {{0.0f, 0.0f, 0.0f, 1.0f}}
                }}
            });
            
            CommandBuffer::Execute(skyboxPack.hdrToCubemapPrimaryCommandBuffer, {skyboxPack.secondaryCommandBuffers[(6*mip)+i]});
            
            RenderPass::EndRenderPass(skyboxPack.hdrToCubemapPrimaryCommandBuffer) ;
        }
    }

    CommandBuffer::End(skyboxPack.hdrToCubemapPrimaryCommandBuffer) ;
}

void Skybox::Submit(ContextInfo& context, SkyboxPack& skyboxPack)
{
    SkyboxInfo& skybox = skyboxPack.skyboxes[skyboxPack.selected];
    
    Queue::SubmitQueue(SubmitQueueConfiguration{
        .fence = skybox.hdrToCubemapFence,
        .graphicsQueue = context.queuePack.graphicsQueue,
        .submissionInfos = std::vector<SubmitQueueInfo>{
            SubmitQueueInfo{
                .waitSemaphores = {},
                .signalSemaphores = {skybox.hdrToCubemapIrradianceSemaphore, skybox.hdrToCubemapPreFilteredEnvironmentSemaphore},
                .commandBuffers = {skyboxPack.hdrToCubemapPrimaryCommandBuffer},
                .waitStages = {}
            }
        }
    });
}


void Skybox::RecordFace(ContextInfo& context, SkyboxPack& skyboxPack, uint32_t mip, uint32_t i)
{
    SkyboxInfo& skybox = skyboxPack.skyboxes[skyboxPack.selected];

    
    VkCommandBuffer commandBuffer = skyboxPack.secondaryCommandBuffers[(6*mip)+i];

    
    CommandBuffer::Begin(commandBuffer, 0, nullptr);
    
    Viewport::Set(commandBuffer, 0, std::vector<VkViewport>{
        VkViewport{
            .x = 0.0f,
            .y = 0.0f,
            .width = static_cast<float>(skybox.hdrToCubemapResolutions[mip].width),
            .height = static_cast<float>(skybox.hdrToCubemapResolutions[mip].height),
            .minDepth = 0.0f,
            .maxDepth = 1.0f
        }
    });
    
    Scissor::SetScissor( SetScissorConfiguration{
        .offset = 0,
        .commandBuffer = commandBuffer,
        .scissors = std::vector<VkRect2D>{
            VkRect2D{
                .offset = { 0, 0 },
                .extent = skybox.hdrToCubemapResolutions[mip]
            }
        }
    }) ;


    Buffer::CopyMemory(CopyMemoryConfiguration{
        .data = &skybox.hdrToCubemapVertexShaderUniformDatas[(6*mip)+i],
        .dataSize = sizeof(HDRToCubemapVertexShaderUniformVariables),
        .memoryPointer = skybox.hdrToCubemapUniformBuffers[(6*mip)+i].memoryPointer
    });
    
    
    GraphicsPipeline::Bind(commandBuffer, skyboxPack.graphicsPipelines[HDRTOCUBEMAP].handle) ;
    
    
    DescriptorSet::Bind(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, skyboxPack.pipelineLayouts[HDRTOCUBEMAP], {skybox.hdrToCubemapDescriptorSets[(6*mip)+i]});
        
    Buffer::BindVertexBuffer(BindVertexBufferConfiguration{
        .offsets = {0},
        .firstBinding = 0,
        .commandBuffer = commandBuffer,
        .vertexBuffers = {skybox.skyboxVertexBufferInfo.buffer}
    }) ;
    
    
    Drawer::Draw(commandBuffer, static_cast<uint32_t>(skyboxVertices.size()), 1, 0, 0);
    
    CommandBuffer::End(commandBuffer) ;
}


void Skybox::Render(SceneInfo& scene, uint32_t frame)
{
    if(scene.skyboxPack.skyboxes.size() == 0)
        return;
    
    SkyboxPack& skyboxPack = scene.skyboxPack;
    
    SkyboxInfo& skybox = skyboxPack.skyboxes[skyboxPack.selected];
    
    if(!skybox.ready)
        return;
    
    VkCommandBuffer sceneSecondaryCommandBuffer = scene.primaryCommandBuffers[frame];
    
    GraphicsPipeline::Bind(sceneSecondaryCommandBuffer, skyboxPack.graphicsPipelines[SKYBOX].handle);
    
    Buffer::CopyMemory(CopyMemoryConfiguration{
        .data = &skybox.vsData,
        .dataSize = sizeof(SkyboxVertexShaderUniformVariables),
        .memoryPointer = skybox.skyboxUniformBufferInfo[frame].memoryPointer
    });
    
    DescriptorSet::Bind(sceneSecondaryCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, skyboxPack.pipelineLayouts[SKYBOX], {skybox.skyboxDescriptorSets[frame]});
    
    Buffer::BindVertexBuffer(BindVertexBufferConfiguration{
        .commandBuffer = sceneSecondaryCommandBuffer,
        .vertexBuffers = {skybox.skyboxVertexBufferInfo.buffer},
        .firstBinding = 0,
        .offsets = {0}
    }) ;

    Drawer::Draw(sceneSecondaryCommandBuffer, static_cast<uint32_t>(skyboxVertices.size()), 1, 0, 0);
}
