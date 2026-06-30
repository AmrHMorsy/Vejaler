#pragma once
#include "../../../Includes.h"
#include "DepthPrepassShaderUniform.h"


const std::string depthPrepassVSFilePath = "../Resources/Shaders/DepthPrepass/DepthPrepass_vs.spv";

const ShaderConfig depthPrepassShaderConfig{
    .filePaths = {depthPrepassVSFilePath},
    .stageFlags = {VK_SHADER_STAGE_VERTEX_BIT}
};

const SamplerConfig depthPrepassSamplerConfig{
    .magFilter = VK_FILTER_NEAREST,
    .minFilter = VK_FILTER_NEAREST,
    .addressMode = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
    .anisotropyEnable = VK_FALSE,
    .borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
    .unnormalizedCoordinates = VK_FALSE,
    .compareEnable = VK_FALSE,
    .compareOp = VK_COMPARE_OP_ALWAYS,
    .mipMapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST,
    .mipLodBias = 0.0f,
    .minLod = 0.0f,
    .maxLod = 0.0f
};

const std::vector<DescriptorBinding> depthPrepassDescriptorBindings = {
    DescriptorBinding{
        .binding = 0,
        .descriptorActualCount = 1,
        .descriptorMaxCapacity = 1,
        .stageFlag = VK_SHADER_STAGE_VERTEX_BIT,
        .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .descriptorImages = {VkDescriptorImageInfo{}},
        .descriptorBuffers = {VkDescriptorBufferInfo{}},
        .bufferSize = sizeof(DepthPrepassShaderUniform::Vertex)
    },
    DescriptorBinding{
        .binding = 1,
        .descriptorActualCount = 1,
        .descriptorMaxCapacity = 1,
        .stageFlag = VK_SHADER_STAGE_VERTEX_BIT,
        .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
        .descriptorImages = {VkDescriptorImageInfo{}},
        .descriptorBuffers = {VkDescriptorBufferInfo{}}
    }
};

const GraphicsPipelineConfig depthPrepassGraphicsPipelineConfig{
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
        .depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL,
        .stencilTestEnable = VK_FALSE,
        .depthBoundsTestEnable = VK_FALSE
    },
    .inputAssemblyStateConfig = {
        .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        .primitiveRestartEnable = VK_FALSE
    },
};
