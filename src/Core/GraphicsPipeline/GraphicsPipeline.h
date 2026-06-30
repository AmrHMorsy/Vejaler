#pragma once
#include "../../Includes.h"
#include "Data/GraphicsPipelineInfo.h"
#include "../Shader/Data/ShaderInfo.h"
#include "Data/GraphicsPipelineConfig.h"
#include "../VertexInput/Data/VertexInputInfo.h"


class GraphicsPipeline{
    
public:
    
    static GraphicsPipelineInfo Build(VkDevice logicalDevice, const GraphicsPipelineConfig& config, VkRenderPass& renderPass, VkPipelineLayout& pipelineLayout, ShaderInfo& shader, VertexInputInfo& vertexInput);
    static void Bind(VkCommandBuffer commandBuffer, VkPipeline graphicsPipeline);
    static void Destroy(VkDevice logicalDevice, VkPipeline& graphicsPipeline);
};
