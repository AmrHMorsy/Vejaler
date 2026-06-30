#pragma once
#include "../../../Includes.h"




struct GraphicsPipelineConfig{
    std::vector<VkDynamicState> dynamicStates;
    std::vector<VkPipelineShaderStageCreateInfo> shaderStages;

    struct ViewportStateConfig{
        uint32_t scissorCount;
        uint32_t viewportCount;
    } viewportStateConfig;
        
    struct ColorBlendStateConfig{
        VkLogicOp logicOp ;
        VkBool32 logicOpEnable ;
        glm::vec4 blendConstants ;
        
        struct ColorBlendAttachmentConfig{
            VkBool32 blendEnable;
            VkBlendOp colorBlendOp;
            VkBlendOp alphaBlendOp;
            VkBlendFactor srcAlphaBlendFactor;
            VkBlendFactor dstAlphaBlendFactor;
            VkBlendFactor srcColorBlendFactor;
            VkBlendFactor dstColorBlendFactor;
            VkColorComponentFlags colorWriteMask;
        };

        std::vector<ColorBlendAttachmentConfig> colorBlendAttachmentConfigs;
    } colorBlendStateConfig;

    struct DepthStencilStateConfig{
        uint32_t depthTestEnable ;
        uint32_t depthWriteEnable ;
        VkCompareOp depthCompareOp ;
        VkBool32 stencilTestEnable ;
        VkBool32 depthBoundsTestEnable ;
    } depthStencilStateConfig;

    struct InputAssemblyStateConfig{
        VkPrimitiveTopology topology ;
        VkBool32 primitiveRestartEnable ;
    } inputAssemblyStateConfig;

    struct RasterizerStateConfig{
        float lineWidth;
        float depthBiasClamp;
        VkFrontFace frontFace;
        VkBool32 depthBiasEnable;
        VkCullModeFlags cullMode;
        VkBool32 depthClampEnable;
        VkPolygonMode polygonMode;
        float depthBiasSlopeFactor;
        float depthBiasConstantFactor;
        VkBool32 rasterizerDiscardEnable;
    } rasterizerStateConfig;

    struct MultisampleStateConfig{
        float minSampleShading;
        VkBool32 alphaToOneEnable;
        VkBool32 sampleShadingEnable;
        VkBool32 alphaToCoverageEnable;
        const VkSampleMask * pSampleMask;
        VkSampleCountFlagBits rasterizationSamples;
    } multisampleStateConfig;
};
