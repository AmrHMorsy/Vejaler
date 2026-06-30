#pragma once
#include "../../../Includes.h"
#include "SSAOShaderUniform.h"
#include "../../../Core/Image/Data/ImageInfo.h"
#include "../../../Core/Shader/Data/ShaderInfo.h"
#include "../../../Core/Buffer/Data/UniformBufferPack.h"
#include "../../../Core/VertexInput/Data/VertexInputInfo.h"
#include "../../../Core/GraphicsPipeline/Data/GraphicsPipelineInfo.h"
#include "../../../Core/DescriptorBinding/Data/DescriptorBinding.h"
#include "../../Texture/Data/TexturePack.h"



const VkExtent2D noiseTextureResolution{
    .width = 4,
    .height = 4
};
const uint32_t noiseTextureImageSize = noiseTextureResolution.width * noiseTextureResolution.height * 4 * sizeof(uint16_t);


struct SSAOInfo{
    ShaderInfo shader;
    uint16_t * noiseData;
    VkRenderPass renderPass;
    TexturePack noiseTexture;
    std::vector<VkFence> fences;
    VertexInputInfo vertexInput;
    std::vector<ImageInfo> images;
    VkDescriptorPool descriptorPool;
    VkPipelineLayout pipelineLayout;
    VkCommandPool primaryCommandPool;
    std::vector<VkImageView> imageViews;
    GraphicsPipelineInfo graphicsPipeline;
    std::vector<VkFramebuffer> frameBuffers;
    VkDescriptorSetLayout descriptorSetLayout;
    std::vector<VkSemaphore> finishedSemaphores;
    std::vector<VkDescriptorSet> descriptorSets;
    GraphicsPipelineConfig graphicsPipelineConfig;
    std::array<glm::vec4, NUM_SAMPLES> samplingKernel;
    std::vector<VkCommandBuffer> primaryCommandBuffers;
    SSAOShaderUniform::Fragment fsData;
    std::vector<UniformBufferPack> fragmentShaderUniformBuffers;
    std::vector<std::vector<DescriptorBinding>> descriptorBindings;
};
