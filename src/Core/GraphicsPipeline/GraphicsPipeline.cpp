#include "GraphicsPipeline.h"




GraphicsPipelineInfo GraphicsPipeline::Build(VkDevice logicalDevice, const GraphicsPipelineConfig& config, VkRenderPass& renderPass, VkPipelineLayout& pipelineLayout, ShaderInfo& shader, VertexInputInfo& vertexInput)
{
    GraphicsPipelineInfo graphicsPipeline;
    
    graphicsPipeline.config = config;
    
    
    graphicsPipeline.dynamicStateCreate = {} ;
    graphicsPipeline.dynamicStateCreate.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    graphicsPipeline.dynamicStateCreate.dynamicStateCount = static_cast<uint32_t>(graphicsPipeline.config.dynamicStates.size());
    graphicsPipeline.dynamicStateCreate.pDynamicStates = graphicsPipeline.config.dynamicStates.data();


    graphicsPipeline.depthStencilStateCreate = {};
    graphicsPipeline.depthStencilStateCreate.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    graphicsPipeline.depthStencilStateCreate.depthTestEnable = graphicsPipeline.config.depthStencilStateConfig.depthTestEnable;
    graphicsPipeline.depthStencilStateCreate.depthWriteEnable = graphicsPipeline.config.depthStencilStateConfig.depthWriteEnable;
    graphicsPipeline.depthStencilStateCreate.depthCompareOp = graphicsPipeline.config.depthStencilStateConfig.depthCompareOp;
    graphicsPipeline.depthStencilStateCreate.depthBoundsTestEnable = graphicsPipeline.config.depthStencilStateConfig.depthBoundsTestEnable;
    graphicsPipeline.depthStencilStateCreate.stencilTestEnable = graphicsPipeline.config.depthStencilStateConfig.stencilTestEnable;


    size_t n = graphicsPipeline.config.colorBlendStateConfig.colorBlendAttachmentConfigs.size();
    graphicsPipeline.colorBlendAttachments.resize(n);
    for(size_t i = 0; i < n; i++){
        graphicsPipeline.colorBlendAttachments[i].colorWriteMask = graphicsPipeline.config.colorBlendStateConfig.colorBlendAttachmentConfigs[i].colorWriteMask ;
        graphicsPipeline.colorBlendAttachments[i].blendEnable = graphicsPipeline.config.colorBlendStateConfig.colorBlendAttachmentConfigs[i].blendEnable;
        graphicsPipeline.colorBlendAttachments[i].srcColorBlendFactor = graphicsPipeline.config.colorBlendStateConfig.colorBlendAttachmentConfigs[i].srcColorBlendFactor;
        graphicsPipeline.colorBlendAttachments[i].dstColorBlendFactor = graphicsPipeline.config.colorBlendStateConfig.colorBlendAttachmentConfigs[i].dstColorBlendFactor;
        graphicsPipeline.colorBlendAttachments[i].colorBlendOp = graphicsPipeline.config.colorBlendStateConfig.colorBlendAttachmentConfigs[i].colorBlendOp;
        graphicsPipeline.colorBlendAttachments[i].srcAlphaBlendFactor = graphicsPipeline.config.colorBlendStateConfig.colorBlendAttachmentConfigs[i].srcAlphaBlendFactor;
        graphicsPipeline.colorBlendAttachments[i].dstAlphaBlendFactor = graphicsPipeline.config.colorBlendStateConfig.colorBlendAttachmentConfigs[i].dstAlphaBlendFactor;
        graphicsPipeline.colorBlendAttachments[i].alphaBlendOp = graphicsPipeline.config.colorBlendStateConfig.colorBlendAttachmentConfigs[i].alphaBlendOp;
    }


    graphicsPipeline.colorBlendStateCreate = {};
    graphicsPipeline.colorBlendStateCreate.attachmentCount = static_cast<uint32_t>(graphicsPipeline.colorBlendAttachments.size()) ;
    graphicsPipeline.colorBlendStateCreate.pAttachments = graphicsPipeline.colorBlendAttachments.data();
    graphicsPipeline.colorBlendStateCreate.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    graphicsPipeline.colorBlendStateCreate.logicOpEnable = graphicsPipeline.config.colorBlendStateConfig.logicOpEnable ;
    graphicsPipeline.colorBlendStateCreate.logicOp = graphicsPipeline.config.colorBlendStateConfig.logicOp ;
    for(int i = 0; i < 4; i++)
        graphicsPipeline.colorBlendStateCreate.blendConstants[i] = graphicsPipeline.config.colorBlendStateConfig.blendConstants[i];


    graphicsPipeline.inputAssemblyStateCreate = {};
    graphicsPipeline.inputAssemblyStateCreate.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO ;
    graphicsPipeline.inputAssemblyStateCreate.topology = graphicsPipeline.config.inputAssemblyStateConfig.topology ;
    graphicsPipeline.inputAssemblyStateCreate.primitiveRestartEnable = graphicsPipeline.config.inputAssemblyStateConfig.primitiveRestartEnable;


    graphicsPipeline.rasterizationStateCreate = {};
    graphicsPipeline.rasterizationStateCreate.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    graphicsPipeline.rasterizationStateCreate.depthClampEnable = graphicsPipeline.config.rasterizerStateConfig.depthClampEnable ;
    graphicsPipeline.rasterizationStateCreate.rasterizerDiscardEnable = graphicsPipeline.config.rasterizerStateConfig.rasterizerDiscardEnable;
    graphicsPipeline.rasterizationStateCreate.polygonMode = graphicsPipeline.config.rasterizerStateConfig.polygonMode;
    graphicsPipeline.rasterizationStateCreate.lineWidth = graphicsPipeline.config.rasterizerStateConfig.lineWidth ;
    graphicsPipeline.rasterizationStateCreate.cullMode = graphicsPipeline.config.rasterizerStateConfig.cullMode ;
    graphicsPipeline.rasterizationStateCreate.frontFace = graphicsPipeline.config.rasterizerStateConfig.frontFace ;
    graphicsPipeline.rasterizationStateCreate.depthBiasEnable = graphicsPipeline.config.rasterizerStateConfig.depthBiasEnable ;
    graphicsPipeline.rasterizationStateCreate.depthBiasConstantFactor = graphicsPipeline.config.rasterizerStateConfig.depthBiasConstantFactor ;
    graphicsPipeline.rasterizationStateCreate.depthBiasClamp = graphicsPipeline.config.rasterizerStateConfig.depthBiasClamp ;
    graphicsPipeline.rasterizationStateCreate.depthBiasSlopeFactor = graphicsPipeline.config.rasterizerStateConfig.depthBiasSlopeFactor ;


    graphicsPipeline.multisampleStateCreate = {};
    graphicsPipeline.multisampleStateCreate.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO ;
    graphicsPipeline.multisampleStateCreate.sampleShadingEnable = graphicsPipeline.config.multisampleStateConfig.sampleShadingEnable ;
    graphicsPipeline.multisampleStateCreate.rasterizationSamples = graphicsPipeline.config.multisampleStateConfig.rasterizationSamples ;
    graphicsPipeline.multisampleStateCreate.minSampleShading = graphicsPipeline.config.multisampleStateConfig.minSampleShading;
    graphicsPipeline.multisampleStateCreate.pSampleMask = graphicsPipeline.config.multisampleStateConfig.pSampleMask ;
    graphicsPipeline.multisampleStateCreate.alphaToCoverageEnable = graphicsPipeline.config.multisampleStateConfig.alphaToCoverageEnable ;
    graphicsPipeline.multisampleStateCreate.alphaToOneEnable = graphicsPipeline.config.multisampleStateConfig.alphaToOneEnable;


    graphicsPipeline.vertexInputStateCreate = {};
    graphicsPipeline.vertexInputStateCreate.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO ;
    graphicsPipeline.vertexInputStateCreate.vertexBindingDescriptionCount = static_cast<uint32_t>(vertexInput.vertexBindingDescription.size());
    graphicsPipeline.vertexInputStateCreate.pVertexBindingDescriptions = vertexInput.vertexBindingDescription.data();
    graphicsPipeline.vertexInputStateCreate.vertexAttributeDescriptionCount = static_cast<uint32_t>(vertexInput.vertexAttributeDescription.size());
    graphicsPipeline.vertexInputStateCreate.pVertexAttributeDescriptions = vertexInput.vertexAttributeDescription.data();


    graphicsPipeline.viewportStateCreate = {};
    graphicsPipeline.viewportStateCreate.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO ;
    graphicsPipeline.viewportStateCreate.viewportCount = graphicsPipeline.config.viewportStateConfig.viewportCount;
    graphicsPipeline.viewportStateCreate.scissorCount = graphicsPipeline.config.viewportStateConfig.scissorCount;


    graphicsPipeline.graphicsPipelineCreate = {};
    graphicsPipeline.graphicsPipelineCreate.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    graphicsPipeline.graphicsPipelineCreate.stageCount = static_cast<uint32_t>(shader.pipelineShaderStages.size());
    graphicsPipeline.graphicsPipelineCreate.pStages = shader.pipelineShaderStages.data();
    graphicsPipeline.graphicsPipelineCreate.pVertexInputState = &graphicsPipeline.vertexInputStateCreate;
    graphicsPipeline.graphicsPipelineCreate.pInputAssemblyState = &graphicsPipeline.inputAssemblyStateCreate;
    graphicsPipeline.graphicsPipelineCreate.pViewportState = &graphicsPipeline.viewportStateCreate;
    graphicsPipeline.graphicsPipelineCreate.pRasterizationState = &graphicsPipeline.rasterizationStateCreate;
    graphicsPipeline.graphicsPipelineCreate.pMultisampleState = &graphicsPipeline.multisampleStateCreate;
    graphicsPipeline.graphicsPipelineCreate.pDepthStencilState = &graphicsPipeline.depthStencilStateCreate;
    graphicsPipeline.graphicsPipelineCreate.pColorBlendState = &graphicsPipeline.colorBlendStateCreate;
    graphicsPipeline.graphicsPipelineCreate.pDynamicState = &graphicsPipeline.dynamicStateCreate;
    graphicsPipeline.graphicsPipelineCreate.layout = pipelineLayout;
    graphicsPipeline.graphicsPipelineCreate.renderPass = renderPass;
    graphicsPipeline.graphicsPipelineCreate.subpass = 0;
    graphicsPipeline.graphicsPipelineCreate.basePipelineHandle = VK_NULL_HANDLE;
    
    
    VkResult result = vkCreateGraphicsPipelines(logicalDevice, VK_NULL_HANDLE, 1, &graphicsPipeline.graphicsPipelineCreate, nullptr, &graphicsPipeline.handle);
    if(result != VK_SUCCESS)
        throw std::runtime_error("\033[31m\nERROR::FAILURE TO CREATE GRAPHICS PIPELINE\033[0m");
    
    return graphicsPipeline;
}

void GraphicsPipeline::Bind(VkCommandBuffer commandBuffer, VkPipeline graphicsPipeline)
{
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
}

void GraphicsPipeline::Destroy(VkDevice logicalDevice, VkPipeline& graphicsPipeline)
{
    vkDestroyPipeline(logicalDevice, graphicsPipeline, nullptr) ;
}
