#pragma once
#include "ModelShaderUniform.h"
#include "../../../Core/DescriptorBinding/Data/DescriptorBinding.h"
#define LINEAR_REPEAT_FLOAT_OPAQUE_WHITE_MIPMAPPED_SAMPLER 0
#define NEAREST_CLAMP_INT_OPAQUE_BLACK_SAMPLER 1
#define LINEAR_CLAMP_INT_OPAQUE_BLACK_SAMPLER 2
#define LINEAR_REPEAT_FLOAT_OPAQUE_WHITE_SAMPLER 3
#define LINEAR_REPEAT_INT_OPAQUE_BLACK_MIPMAPPED_SAMPLER 4
#define PBR 0
#define UNLIT 1
#define ALBEDO 0
#define NORMAL 1
#define ROUGHNESS 2
#define METALLIC 3


const uint32_t maxMipLevelCount = static_cast<uint32_t>(std::floor(std::log2(std::max(4096, 4096)))) + 1;

const SamplerConfig linearRepeatFloatOpaqueWhiteMipMappedSampler{
    .magFilter = VK_FILTER_LINEAR,
    .minFilter = VK_FILTER_LINEAR,
    .addressMode = VK_SAMPLER_ADDRESS_MODE_REPEAT,
    .anisotropyEnable = VK_FALSE,
    .borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE,
    .unnormalizedCoordinates = VK_FALSE,
    .compareEnable = VK_FALSE,
    .compareOp = VK_COMPARE_OP_ALWAYS,
    .mipMapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,
    .mipLodBias = 0.0f,
    .minLod = 0.0f,
    .maxLod = float(maxMipLevelCount)
};

const SamplerConfig nearestClampIntOpaqueBlackSampler{
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

const SamplerConfig linearClampIntOpaqueBlackSampler{
    .magFilter = VK_FILTER_LINEAR,
    .minFilter = VK_FILTER_LINEAR,
    .addressMode = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
    .anisotropyEnable = VK_FALSE,
    .borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
    .unnormalizedCoordinates = VK_FALSE,
    .compareEnable = VK_FALSE,
    .compareOp = VK_COMPARE_OP_ALWAYS,
    .mipMapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,
    .mipLodBias = 0.0f,
    .minLod = 0.0f,
    .maxLod = 0.0f
};

const SamplerConfig linearRepeatFloatOpaqueWhiteSampler{
    .magFilter = VK_FILTER_LINEAR,
    .minFilter = VK_FILTER_LINEAR,
    .addressMode = VK_SAMPLER_ADDRESS_MODE_REPEAT,
    .anisotropyEnable = VK_FALSE,
    .borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE,
    .unnormalizedCoordinates = VK_FALSE,
    .compareEnable = VK_FALSE,
    .compareOp = VK_COMPARE_OP_ALWAYS,
    .mipMapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,
    .mipLodBias = 0.0f,
    .minLod = 0.0f,
    .maxLod = 0.0f
};

const SamplerConfig linearRepeatIntOpaqueBlackMipMappedSampler{
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
    .maxLod = float(maxMipLevelCount)
};

const std::vector<SamplerConfig> samplerConfigs{
    linearRepeatFloatOpaqueWhiteMipMappedSampler,
    nearestClampIntOpaqueBlackSampler,
    linearClampIntOpaqueBlackSampler,
    linearRepeatFloatOpaqueWhiteSampler,
    linearRepeatIntOpaqueBlackMipMappedSampler
};

const ShaderConfig modelShaderConfig{
    .filePaths = {
        "../Resources/shaders/PBR/vert.spv",
        "../Resources/shaders/PBR/frag.spv"
    },
    .stageFlags{
        VK_SHADER_STAGE_VERTEX_BIT,
        VK_SHADER_STAGE_FRAGMENT_BIT
    }
};

const std::vector<DescriptorBinding> modelDescriptorBindingSchematic1{
    DescriptorBinding{
        .binding = 0,
        .descriptorActualCount = 1,
        .descriptorMaxCapacity = 1,
        .stageFlag = VK_SHADER_STAGE_VERTEX_BIT,
        .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .descriptorImages = {VkDescriptorImageInfo{}},
        .descriptorBuffers = {VkDescriptorBufferInfo{}},
        .bufferSize = sizeof(ModelShaderUniform::Vertex)
    },
    DescriptorBinding{
        .binding = 1,
        .descriptorActualCount = 1,
        .descriptorMaxCapacity = 1,
        .stageFlag = VK_SHADER_STAGE_FRAGMENT_BIT,
        .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .descriptorImages = {VkDescriptorImageInfo{}},
        .descriptorBuffers = {VkDescriptorBufferInfo{}},
        .bufferSize = sizeof(ModelShaderUniform::Fragment)
    },
    DescriptorBinding{
        .binding = 2,
        .descriptorMaxCapacity = 60,
        .stageFlag = VK_SHADER_STAGE_FRAGMENT_BIT,
        .descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
        .descriptorImages = {VkDescriptorImageInfo{}},
        .descriptorBuffers = {VkDescriptorBufferInfo{}},
        .bufferSize = 0,
        .descriptorBindingFlags = VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT | VK_DESCRIPTOR_BINDING_VARIABLE_DESCRIPTOR_COUNT_BIT
    }
};

const std::vector<DescriptorBinding> modelDescriptorBindingSchematic2{
    DescriptorBinding{
        .binding = 0,
        .descriptorActualCount = 1,
        .descriptorMaxCapacity = 1,
        .stageFlag = VK_SHADER_STAGE_FRAGMENT_BIT,
        .descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER,
        .descriptorImages = {VkDescriptorImageInfo{}},
        .descriptorBuffers = {VkDescriptorBufferInfo{}},
        .bufferSize = 0,
        .descriptorBindingFlags = 0
    },
    DescriptorBinding{
        .binding = 1,
        .descriptorMaxCapacity = MAX_NUM_LIGHTS,
        .stageFlag = VK_SHADER_STAGE_FRAGMENT_BIT,
        .descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
        .descriptorImages = {VkDescriptorImageInfo{}},
        .descriptorBuffers = {VkDescriptorBufferInfo{}},
        .bufferSize = 0,
        .descriptorBindingFlags = VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT | VK_DESCRIPTOR_BINDING_VARIABLE_DESCRIPTOR_COUNT_BIT
    }
};

const std::vector<DescriptorBinding> modelDescriptorBindingSchematic3{
    DescriptorBinding{
        .binding = 0,
        .descriptorActualCount = 1,
        .descriptorMaxCapacity = 1,
        .stageFlag = VK_SHADER_STAGE_FRAGMENT_BIT,
        .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
        .descriptorImages = {VkDescriptorImageInfo{}},
        .descriptorBuffers = {VkDescriptorBufferInfo{}}
    },
    DescriptorBinding{
        .binding = 1,
        .descriptorActualCount = 1,
        .descriptorMaxCapacity = 1,
        .stageFlag = VK_SHADER_STAGE_FRAGMENT_BIT,
        .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
        .descriptorImages = {VkDescriptorImageInfo{}},
        .descriptorBuffers = {VkDescriptorBufferInfo{}}
    },
    DescriptorBinding{
        .binding = 2,
        .descriptorActualCount = 1,
        .descriptorMaxCapacity = 1,
        .stageFlag = VK_SHADER_STAGE_FRAGMENT_BIT,
        .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
        .descriptorImages = {VkDescriptorImageInfo{}},
        .descriptorBuffers = {VkDescriptorBufferInfo{}}
    }
};

const std::vector<DescriptorBinding> modelDescriptorBindingSchematic4{
    DescriptorBinding{
        .binding = 0,
        .descriptorActualCount = 1,
        .descriptorMaxCapacity = 1,
        .stageFlag = VK_SHADER_STAGE_FRAGMENT_BIT,
        .descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
        .descriptorImages = {VkDescriptorImageInfo{}},
        .descriptorBuffers = {VkDescriptorBufferInfo{}},
        .bufferSize = 0
    },
    DescriptorBinding{
        .binding = 1,
        .descriptorActualCount = 1,
        .descriptorMaxCapacity = 1,
        .stageFlag = VK_SHADER_STAGE_FRAGMENT_BIT,
        .descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
        .descriptorImages = {VkDescriptorImageInfo{}},
        .descriptorBuffers = {VkDescriptorBufferInfo{}},
        .bufferSize = 0
    },
    DescriptorBinding{
        .binding = 2,
        .descriptorActualCount = NUM_SAMPLERS,
        .descriptorMaxCapacity = NUM_SAMPLERS,
        .stageFlag = VK_SHADER_STAGE_FRAGMENT_BIT,
        .descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER,
        .descriptorImages = {VkDescriptorImageInfo{}},
        .descriptorBuffers = {VkDescriptorBufferInfo{}},
        .bufferSize = 0,
        .descriptorBindingFlags = 0
    }
};

const std::vector<DescriptorBinding> modelDescriptorBindingSchematic5{
    DescriptorBinding{
        .binding = 0,
        .descriptorActualCount = 1,
        .descriptorMaxCapacity = 1,
        .stageFlag = VK_SHADER_STAGE_FRAGMENT_BIT,
        .descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
        .descriptorImages = {VkDescriptorImageInfo{}},
        .descriptorBuffers = {VkDescriptorBufferInfo{}},
        .bufferSize = 0
    },
    DescriptorBinding{
        .binding = 1,
        .descriptorActualCount = 1,
        .descriptorMaxCapacity = 1,
        .stageFlag = VK_SHADER_STAGE_FRAGMENT_BIT,
        .descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
        .descriptorImages = {VkDescriptorImageInfo{}},
        .descriptorBuffers = {VkDescriptorBufferInfo{}},
        .bufferSize = 0
    },
    DescriptorBinding{
        .binding = 2,
        .descriptorActualCount = 1,
        .descriptorMaxCapacity = 1,
        .stageFlag = VK_SHADER_STAGE_FRAGMENT_BIT,
        .descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
        .descriptorImages = {VkDescriptorImageInfo{}},
        .descriptorBuffers = {VkDescriptorBufferInfo{}},
        .bufferSize = 0
    }
};

const std::vector<DescriptorBinding> modelDescriptorBindingSchematic6{
    DescriptorBinding{
        .binding = 0,
        .descriptorActualCount = 1,
        .descriptorMaxCapacity = 1,
        .stageFlag = VK_SHADER_STAGE_VERTEX_BIT,
        .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
        .descriptorImages = {VkDescriptorImageInfo{}},
        .descriptorBuffers = {VkDescriptorBufferInfo{}}
    },
    DescriptorBinding{
        .binding = 1,
        .descriptorActualCount = 1,
        .descriptorMaxCapacity = 1,
        .stageFlag = VK_SHADER_STAGE_FRAGMENT_BIT,
        .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
        .descriptorImages = {VkDescriptorImageInfo{}},
        .descriptorBuffers = {VkDescriptorBufferInfo{}}
    }
};

const std::vector<std::vector<DescriptorBinding>> modelDescriptorBindingSchematics{
    modelDescriptorBindingSchematic1,
    modelDescriptorBindingSchematic2,
    modelDescriptorBindingSchematic3,
    modelDescriptorBindingSchematic4,
    modelDescriptorBindingSchematic5,
    modelDescriptorBindingSchematic6
};

const GraphicsPipelineConfig graphicsPipelineConfig{
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
                .blendEnable = VK_TRUE,
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
        .depthCompareOp = VK_COMPARE_OP_EQUAL,
        .stencilTestEnable = VK_FALSE,
        .depthBoundsTestEnable = VK_FALSE
    },
    .inputAssemblyStateConfig = {
        .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        .primitiveRestartEnable = VK_FALSE
    }
};

const GraphicsPipelineConfig transparentGraphicsPipelineConfig{
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
        .cullMode = VK_CULL_MODE_NONE, //VK_CULL_MODE_BACK_BIT,
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
                .blendEnable = VK_TRUE,
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
