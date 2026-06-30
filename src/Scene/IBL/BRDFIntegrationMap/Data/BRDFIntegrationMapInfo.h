#pragma once
#include "../../../../includes.h"
#include "../../../Texture/Data/TexturePack.h"
#include "../../../../Core/Buffer/Data/BufferPack.h"
#include "../../../../Core/GraphicsPipeline/Data/GraphicsPipelineConfig.h"



struct BRDFIntegrationMapInfo{
    VkFence fence;
    ShaderInfo shader;
    ImageInfo image;
    VkImageView imageView;
    VkExtent2D resolution;
    VkRenderPass renderPass;
    BufferPack vertexBuffer;
    VkFramebuffer frameBuffer;
    VertexInputInfo vertexInput;
    VkPipelineLayout pipelineLayout;
    VkCommandPool primaryCommandPool;
    BufferPack textureCoordinateBuffer;
    VkCommandBuffer primaryCommandBuffer;
    GraphicsPipelineInfo graphicsPipeline;
    GraphicsPipelineConfig graphicsPipelineConfig;
};
