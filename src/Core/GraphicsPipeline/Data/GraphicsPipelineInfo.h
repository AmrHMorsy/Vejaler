#pragma once
#include "../../../Includes.h"
#include "GraphicsPipelineConfig.h"




struct GraphicsPipelineInfo{
    VkPipeline handle;
    GraphicsPipelineConfig config;
    VkPipelineDynamicStateCreateInfo dynamicStateCreate;
    VkGraphicsPipelineCreateInfo graphicsPipelineCreate;
    VkPipelineViewportStateCreateInfo viewportStateCreate;
    VkPipelineColorBlendStateCreateInfo colorBlendStateCreate;
    std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
    VkPipelineMultisampleStateCreateInfo multisampleStateCreate;
    VkPipelineVertexInputStateCreateInfo vertexInputStateCreate;
    VkPipelineDepthStencilStateCreateInfo depthStencilStateCreate;
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreate;
    VkPipelineRasterizationStateCreateInfo rasterizationStateCreate;
    std::vector<VkPipelineColorBlendAttachmentState> colorBlendAttachments;
};
